
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

class SeulLaneDetection
{
public: 
    SeulLaneDetection(cv::Mat& src);
    void seulResize(cv::Mat& src, int num, std::string optr);
    void seulMakeHSVTrackbar(const std::string &trackbar_name);
private:
    cv::Mat src_frame;
};

