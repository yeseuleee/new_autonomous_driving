
#include <img_processing/seul_lane_detection/seul_lane_detection.h>
//main cam static value declare



SeulLaneDetection::SeulLaneDetection(cv::Mat &src_, const ros::NodeHandle &nh_, const std::string &group_name_, std::vector<int>& y_lane_arr_, std::vector<int>& w_lane_arr_)
    : src(src_), nh(nh_), group_name(group_name_),y_lane_arr(y_lane_arr_),w_lane_arr(w_lane_arr_),    
      trackbar_name_y(group_name + "_yellow_lane_trackbar"), trackbar_name_w(group_name + "_white_lane_trackbar")
{
    if (!src.empty())
    {
        seulResize(2, "/");
        seulMakeHSVTrackbar();
        seulSetHSVTrackbar();
        seulDetectHSVColor();
        seulShowTrackbar();
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
    { //devide img
        cv::resize(src, src, cv::Size(src.cols / num, src.rows / num), 0, 0, CV_INTER_AREA);
    }
    // printf("after resize : %d %d\n",src.cols, src.rows);
}

void SeulLaneDetection::seulMakeHSVTrackbar()
{
    //make yellow lane trackbar
    cv::namedWindow(trackbar_name_y, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("h min", trackbar_name_y, &y_lane_arr[0], 179, NULL);
    cv::setTrackbarPos("h min", trackbar_name_y, y_lane_arr[0]);

    cv::createTrackbar("h max", trackbar_name_y, &y_lane_arr[1], 179, NULL);
    cv::setTrackbarPos("h max", trackbar_name_y, y_lane_arr[1]);

    cv::createTrackbar("s min", trackbar_name_y, &y_lane_arr[2], 255, NULL);
    cv::setTrackbarPos("s min", trackbar_name_y, y_lane_arr[2]);

    cv::createTrackbar("s max", trackbar_name_y, &y_lane_arr[3], 255, NULL);
    cv::setTrackbarPos("s max", trackbar_name_y, y_lane_arr[3]);

    cv::createTrackbar("v min", trackbar_name_y, &y_lane_arr[4], 255, NULL);
    cv::setTrackbarPos("v min", trackbar_name_y, y_lane_arr[4]);

    cv::createTrackbar("v max", trackbar_name_y, &y_lane_arr[5], 255, NULL);
    cv::setTrackbarPos("v max", trackbar_name_y, y_lane_arr[5]);

    //make white lane trackbar
    cv::namedWindow(trackbar_name_w, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("h min", trackbar_name_w, &w_lane_arr[0], 179, NULL);
    cv::setTrackbarPos("h min", trackbar_name_w, w_lane_arr[0]);

    cv::createTrackbar("h max", trackbar_name_w, &w_lane_arr[1], 179, NULL);
    cv::setTrackbarPos("h max", trackbar_name_w, w_lane_arr[1]);

    cv::createTrackbar("s min", trackbar_name_w, &w_lane_arr[2], 255, NULL);
    cv::setTrackbarPos("s min", trackbar_name_w, w_lane_arr[2]);

    cv::createTrackbar("s max", trackbar_name_w, &w_lane_arr[3], 255, NULL);
    cv::setTrackbarPos("s max", trackbar_name_w, w_lane_arr[3]);

    cv::createTrackbar("v min", trackbar_name_w, &w_lane_arr[4], 255, NULL);
    cv::setTrackbarPos("v min", trackbar_name_w, w_lane_arr[4]);

    cv::createTrackbar("v max", trackbar_name_w, &w_lane_arr[5], 255, NULL);
    cv::setTrackbarPos("v max", trackbar_name_w, w_lane_arr[5]);
}

void SeulLaneDetection::seulSetHSVTrackbar()
{
    //get yellow lane trackbar's value
    y_lane_arr[0] = cv::getTrackbarPos("h min", trackbar_name_y);
    y_lane_arr[1] = cv::getTrackbarPos("h max", trackbar_name_y);
    y_lane_arr[2] = cv::getTrackbarPos("s min", trackbar_name_y);
    y_lane_arr[3] = cv::getTrackbarPos("s max", trackbar_name_y);
    y_lane_arr[4] = cv::getTrackbarPos("v min", trackbar_name_y);
    y_lane_arr[5] = cv::getTrackbarPos("v max", trackbar_name_y);

    //get white lane trackbar's value
    w_lane_arr[0] = cv::getTrackbarPos("h min", trackbar_name_w);
    w_lane_arr[1] = cv::getTrackbarPos("h max", trackbar_name_w);
    w_lane_arr[2] = cv::getTrackbarPos("s min", trackbar_name_w);
    w_lane_arr[3] = cv::getTrackbarPos("s max", trackbar_name_w);
    w_lane_arr[4] = cv::getTrackbarPos("v min", trackbar_name_w);
    w_lane_arr[5] = cv::getTrackbarPos("v max", trackbar_name_w);
}

void SeulLaneDetection::seulDetectHSVColor()
{
    //detect color value(h,s,v)
    cv::cvtColor(src, dst_y, CV_BGR2HSV);
    cv::inRange(dst_y, cv::Scalar(y_lane_arr[0], y_lane_arr[2], y_lane_arr[4], 0), cv::Scalar(y_lane_arr[1], y_lane_arr[3], y_lane_arr[5], 0), dst_y);

    cv::cvtColor(src, dst_w, CV_BGR2HSV);
    cv::inRange(dst_w, cv::Scalar(w_lane_arr[0], w_lane_arr[2], w_lane_arr[4], 0), cv::Scalar(w_lane_arr[1], w_lane_arr[3], w_lane_arr[5], 0), dst_w);
}

void SeulLaneDetection::seulShowTrackbar()
{
    //for trackbar visualizing
    cv::imshow(trackbar_name_w, dst_w);
    cv::imshow(trackbar_name_y, dst_y);
}