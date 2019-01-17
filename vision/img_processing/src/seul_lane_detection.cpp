
#include <img_processing/seul_lane_detection/seul_lane_detection.h>

// int SeulLaneDetection::obj_shared_val = 0;

SeulLaneDetection::SeulLaneDetection(cv::Mat &src_, const ros::NodeHandle &nh_, const std::string &group_name_)
    : src(src_), nh(nh_), group_name(group_name_),
      trackbar_name_y(group_name + "_yellow_lane_trackbar"), trackbar_name_w(group_name + "_white_lane_trackbar")
{
    if (!src.empty())
    {
        seulResize(2, "/");
        seulInitLaneParam();
        seulMakeHSVTrackbar();
        seulSetHSVTrackbar();
        seulDetectHSVColor();
       
    }
    else
    {
        std::cout << "frame is empty\n";
    }
}

void SeulLaneDetection::seulResize(int num, std::string optr)
{
    // printf("before resize : %d %d\n",src.cols, src.rows);
    if (optr == "*")
    { //twice img
        cv::resize(src, src, cv::Size(src.cols * num, src.rows * num), 0, 0, CV_INTER_AREA);
    }
    else if (optr == "/")
    { //devide img{
        cv::resize(src, src, cv::Size(src.cols / num, src.rows / num), 0, 0, CV_INTER_AREA);
    }
    // printf("after resize : %d %d\n",src.cols, src.rows);
}
void SeulLaneDetection::seulInitLaneParam()
{
    nh.param<int>("/" + group_name + "/img_processing_node/y_hmin", y_hmin, 10);
    nh.param<int>("/" + group_name + "/img_processing_node/y_hmax", y_hmax, 21);
    nh.param<int>("/" + group_name + "/img_processing_node/y_smin", y_smin, 52);
    nh.param<int>("/" + group_name + "/img_processing_node/y_smax", y_smax, 151);
    nh.param<int>("/" + group_name + "/img_processing_node/y_vmin", y_vmin, 0);
    nh.param<int>("/" + group_name + "/img_processing_node/y_vmax", y_vmax, 180);
    //default white when no launch file for lane color
    nh.param<int>("/" + group_name + "/img_processing_node/w_hmin", w_hmin, 0);
    nh.param<int>("/" + group_name + "/img_processing_node/w_hmax", w_hmax, 180);
    nh.param<int>("/" + group_name + "/img_processing_node/w_smin", w_smin, 0);
    nh.param<int>("/" + group_name + "/img_processing_node/w_smax", w_smax, 24);
    nh.param<int>("/" + group_name + "/img_processing_node/w_vmin", w_vmin, 172);
    nh.param<int>("/" + group_name + "/img_processing_node/w_vmax", w_vmax, 255);
}
void SeulLaneDetection::seulMakeHSVTrackbar()
{
    //make yellow lane trackbar
    cv::namedWindow(trackbar_name_y, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("h min", trackbar_name_y, &y_hmin, 179, NULL);
    cv::setTrackbarPos("h min", trackbar_name_y, y_hmin);

    cv::createTrackbar("h max", trackbar_name_y, &y_hmax, 179, NULL);
    cv::setTrackbarPos("h max", trackbar_name_y, y_hmax);

    cv::createTrackbar("s min", trackbar_name_y, &y_smin, 255, NULL);
    cv::setTrackbarPos("s min", trackbar_name_y, y_smin);

    cv::createTrackbar("s max", trackbar_name_y, &y_smax, 255, NULL);
    cv::setTrackbarPos("s max", trackbar_name_y, y_smax);

    cv::createTrackbar("v min", trackbar_name_y, &y_vmin, 255, NULL);
    cv::setTrackbarPos("v min", trackbar_name_y, y_vmin);

    cv::createTrackbar("v max", trackbar_name_y, &y_vmax, 255, NULL);
    cv::setTrackbarPos("v max", trackbar_name_y, y_vmax);

    //make white lane trackbar
    cv::namedWindow(trackbar_name_w, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("h min", trackbar_name_w, &w_hmin, 179, NULL);
    cv::setTrackbarPos("h min", trackbar_name_w, w_hmin);

    cv::createTrackbar("h max", trackbar_name_w, &w_hmax, 179, NULL);
    cv::setTrackbarPos("h max", trackbar_name_w, w_hmax);

    cv::createTrackbar("s min", trackbar_name_w, &w_smin, 255, NULL);
    cv::setTrackbarPos("s min", trackbar_name_w, w_smin);

    cv::createTrackbar("s max", trackbar_name_w, &w_smax, 255, NULL);
    cv::setTrackbarPos("s max", trackbar_name_w, w_smax);

    cv::createTrackbar("v min", trackbar_name_w, &w_vmin, 255, NULL);
    cv::setTrackbarPos("v min", trackbar_name_w, w_vmin);

    cv::createTrackbar("v max", trackbar_name_w, &w_vmax, 255, NULL);
    cv::setTrackbarPos("v max", trackbar_name_w, w_vmax);
}

void SeulLaneDetection::seulSetHSVTrackbar()
{
    //get yellow lane trackbar's value
    y_hmin = cv::getTrackbarPos("h min", trackbar_name_y);
    y_hmax = cv::getTrackbarPos("h max", trackbar_name_y);
    y_smin = cv::getTrackbarPos("s min", trackbar_name_y);
    y_smax = cv::getTrackbarPos("s max", trackbar_name_y);
    y_vmin = cv::getTrackbarPos("v min", trackbar_name_y);
    y_vmax = cv::getTrackbarPos("v max", trackbar_name_y);
    
    //get white lane trackbar's value
    w_hmin = cv::getTrackbarPos("h min", trackbar_name_w);
    w_hmax = cv::getTrackbarPos("h max", trackbar_name_w);
    w_smin = cv::getTrackbarPos("s min", trackbar_name_w);
    w_smax = cv::getTrackbarPos("s max", trackbar_name_w);
    w_vmin = cv::getTrackbarPos("v min", trackbar_name_w);
    w_vmax = cv::getTrackbarPos("v max", trackbar_name_w);
}

void SeulLaneDetection::seulDetectHSVColor()
{
    cv::cvtColor(src, dst_y, CV_BGR2HSV);
    cv::inRange(dst_y, cv::Scalar(y_hmin, y_smin, y_vmin, 0), cv::Scalar(y_hmax, y_smax, y_vmax, 0), dst_y);
    
    cv::cvtColor(src, dst_w, CV_BGR2HSV);
    cv::inRange(dst_w, cv::Scalar(w_hmin, w_smin, w_vmin, 0), cv::Scalar(w_hmax, w_smax, w_vmax, 0), dst_w);

    cv::imshow(trackbar_name_w,dst_w);
    cv::imshow(trackbar_name_y,dst_y);
        
}