
#include <img_processing/seul_lane_detection/seul_lane_detection.h>


SeulLaneDetection::SeulLaneDetection(cv::Mat& src):src_frame(src){
    if(!src.empty()){
        seulResize(src,2,"/");
    }
    else{
        std::cout<<"frame is empty\n";
    }
}

void SeulLaneDetection::seulResize(cv::Mat& src, int num, std::string optr){
    printf("before resize : %d %d\n",src.cols, src.rows);
    if(optr == "*"){//twice im
        cv::resize(src,src,cv::Size(src.cols*num,src.rows*num),0,0,CV_INTER_AREA);
    }
    else if(optr == "/"){//decrease.. nanugi{
    cv::resize(src,src,cv::Size(src.cols/num,src.rows/num),0,0,CV_INTER_AREA);
    }
    printf("after resize : %d %d\n",src.cols, src.rows);
}

void SeulLaneDetection::seulMakeHSVTrackbar(const std::string& trackbat_name){
    cv::namedWindow(trackbat_name, cv::WINDOW_AUTOSIZE);
     

}
