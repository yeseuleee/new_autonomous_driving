# new_autonomous_driving

<seule_calibration>  
카메라 캘리브레이션 수행하는 클래스입니다.  
캘리브레이션 정보가 적힌 yaml 파일을 읽어 필요한 파라미터를 추출해 가져옵니다.  
(카메라 이미지의 너비,높이,camera_matrix,distortion_coefficients)  
이 정보들을 이용해 본 클래스의 메써드는 initUndistortRectifyMap()을 수행해 remap에 필요한 참조행렬을 생성합니다.  
사용자는 MyRemap 메쏘드에 캘리브레이션하고자 하는 이미지를 파라미터로 전달할 수 있습니다.  

'''c
class SeulCaliMatrix{
    public:
        
        SeulCaliMatrix(int img_col, int img_row, 
                       cv::Mat my_cam_matrix = cv::Mat::eye(3,3,CV_64FC1), 
                       cv::Mat my_dist_coeffs = cv::Mat::zeros(1,5,CV_64FC1))
        : image_width(img_col), image_height(img_row), cam_matrix(my_cam_matrix), dist_coeffs(my_dist_coeffs){
              MyUdistorRectMap(cv::Size(image_width,image_height));
          }

        ...

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
'''
