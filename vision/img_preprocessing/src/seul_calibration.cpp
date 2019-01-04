#include <img_preprocessing/seul_calibration/seul_calibration.h>

SeulCaliMatrix::SeulCaliMatrix(std::string my_file_path)
: file_path(my_file_path){
    cv::FileStorage fs(file_path,cv::FileStorage::READ);    
    if(!fs.isOpened()){std::cerr<<"file read error\n";}
    image_width = (int)fs["image_width"];
    image_height = (int)fs["image_height"];
    cam_matrix = fs["camera_matrix"].mat();
    dist_coeffs = fs["distortion_coefficients"].mat();
    MyUdistorRectMap(cv::Size(image_width,image_height));
}

void SeulCaliMatrix::SetCamMatrix(cv::Mat my_cam_matrix){
    cam_matrix = my_cam_matrix;
}
cv::Mat SeulCaliMatrix::GetCamMatrix(){
    return cam_matrix;
}

void SeulCaliMatrix::SetDistCoeffs(cv::Mat my_sdist_coeffs){
    dist_coeffs = my_sdist_coeffs;
}
cv::Mat SeulCaliMatrix::GetDistCoeffs(){
    return dist_coeffs;
}    
        
void SeulCaliMatrix::MyReMap(cv::Mat& frame){
    cv::remap(frame,frame,map1,map2,cv::INTER_LINEAR);
}
        
void SeulCaliMatrix::MyUdistorRectMap(cv::Size img_size){
    int m1type = CV_32FC1;
    cv::initUndistortRectifyMap(cam_matrix,dist_coeffs,cv::Mat::eye(3, 3, CV_64FC1),
                                cam_matrix,img_size,m1type,map1,map2);
}

