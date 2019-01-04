#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

class SeulCaliMatrix{
    public:
        
        SeulCaliMatrix(std::string my_file_path);

        void SetCamMatrix(cv::Mat my_cam_matrix);        
        cv::Mat GetCamMatrix();

        void SetDistCoeffs(cv::Mat my_sdist_coeffs);
        cv::Mat GetDistCoeffs();
        
        void MyReMap(cv::Mat& frame);
        
    private:
        cv::Mat cam_matrix;
        cv::Mat dist_coeffs;
        cv::Mat map1;
        cv::Mat map2;
        std::string file_path;
        int image_height;   
        int image_width;

        void MyUdistorRectMap(cv::Size img_size);
};