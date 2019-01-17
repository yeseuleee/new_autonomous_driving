
//파라미터 제어 qt

//영상 이진화
//-캐니엣지
//-hsv 분리
//-소벨엣지
//-오쯔

//직선 피팅 및 기울기
//-허프라인
//-임의로 안쪽 선 피팅

//뷰 변환
//-버드아이뷰


//for simulator
//ros토픽 mat으로 바꾸기(생성자 하나 더)

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <ros/ros.h>
class SeulLaneDetection
{
public: 
    SeulLaneDetection(cv::Mat& src_, const ros::NodeHandle& nh_, const std::string& group_name_);
    void seulInitLaneParam();
    void seulResize(int num, std::string optr);
    void seulMakeHSVTrackbar();
    void seulSetHSVTrackbar();
    void seulDetectHSVColor();
 
private:
    cv::Mat& src;
    cv::Mat dst_y, dst_w;
    const ros::NodeHandle& nh;
    const std::string& group_name;
    const std::string& trackbar_name_y, trackbar_name_w;
    int y_hmin, y_hmax, y_smin, y_smax, y_vmin, y_vmax;
    int w_hmin, w_hmax, w_smin, w_smax, w_vmin, w_vmax;
    //static int obj_shared_val;
    //const static int obj_shared_cal = 0;
};

