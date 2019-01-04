#include <pre_process/seul_calibration/seul_calibration.h>
#include <ros/ros.h>
#include <ros/package.h>
#include <iostream>

int main(){
    std::string path = ros::package::getPath("PreProcess")+"/src/camera_main.yaml";
    SeulCaliMatrix cali_img(path);

    cv::VideoCapture cap;
    cv::Mat frame;
    int cam_num = 1;
    cap.open(cam_num);
    while(1){
        cap >> frame;
        cv::imshow("frame before calibration",frame);
        cali_img.MyReMap(frame);
        cv::imshow("frame after calibration",frame);
        int ckey = cv::waitKey(1);
        if(ckey == 27)break;
    }
    return 0;
}