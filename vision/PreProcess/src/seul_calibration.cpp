#include <ros/ros.h>
#include <ros/package.h>
#include <yaml-cpp/yaml.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <string>
using std::cout;
using std::endl; 

class SeulCaliMatrix{
    public:
        
        SeulCaliMatrix(int img_col, int img_row, 
                       cv::Mat my_cam_matrix = cv::Mat::eye(3,3,CV_64FC1), 
                       cv::Mat my_dist_coeffs = cv::Mat::zeros(1,5,CV_64FC1))
        : image_width(img_col), image_height(img_row), cam_matrix(my_cam_matrix), dist_coeffs(my_dist_coeffs){
              MyUdistorRectMap(cv::Size(image_width,image_height));
          }

        void SetCamMatrix(int row=3, int col=3, int type=CV_64FC1){
            cam_matrix = cv::Mat::eye(row,col,type);
        }
        void SetCamMatrix(cv::Mat my_cam_matrix){
            cam_matrix = my_cam_matrix;
        }
        cv::Mat GetCamMatrix(){
            return cam_matrix;
        }

        void SetDistCoeffs(int row=1, int col=5, int type=CV_64FC1){
            dist_coeffs = cv::Mat::zeros(row,col,type);
        }
        void SetDistCoeffs(cv::Mat my_sdist_coeffs){
            dist_coeffs = my_sdist_coeffs;
        }
        cv::Mat GetDistCoeffs(){
            return dist_coeffs;
        }
        
        
        void MyReMap(cv::Mat& frame){
            cv::remap(frame,frame,map1,map2,cv::INTER_LINEAR);
        }
        
    private:
        cv::Mat cam_matrix;
        cv::Mat dist_coeffs;
        cv::Mat map1;
        cv::Mat map2;

        int image_height;   
        int image_width;

        void MyUdistorRectMap(cv::Size img_size){
            int m1type = CV_32FC1;
            cv::initUndistortRectifyMap(cam_matrix,dist_coeffs,cv::Mat::eye(3, 3, CV_64FC1),
                                        cam_matrix,img_size,m1type,map1,map2);
        }
         
};
int main(int argc, char *argv[])
{    
    /*none ros dependency*/
    // std::string path = "./camera_main.yaml";
    std::string path = ros::package::getPath("PreProcess")+"/src/camera_main.yaml";
    cv::FileStorage fs(path,cv::FileStorage::READ);
    if(!fs.isOpened()){std::cerr<<"file read error\n";}

    SeulCaliMatrix cali_img((int)fs["image_width"],(int)fs["image_height"],
                            fs["camera_matrix"].mat(),fs["distortion_coefficients"].mat());

    cv::VideoCapture cap;
    cv::Mat frame;
    cap.open(1);
    cv::Mat map1, map2;
    while(1){
        cap >> frame;
        cali_img.MyReMap(frame);
        int ckey = cv::waitKey(1);
        if(ckey == 27)break;
    }

    return 0;
}
