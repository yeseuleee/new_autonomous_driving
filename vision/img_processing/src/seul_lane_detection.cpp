
#include <img_processing/seul_lane_detection/seul_lane_detection.h>
//main cam static value declare



SeulLaneDetection::SeulLaneDetection(cv::Mat &src_, const std::string &group_name_, std::vector<int>& y_lane_arr_, std::vector<int>& w_lane_arr_)
    : src(src_), group_name(group_name_),y_lane_arr(y_lane_arr_),w_lane_arr(w_lane_arr_),    
      trackbar_name_y(group_name + "_yellow_lane_trackbar"), trackbar_name_w(group_name + "_white_lane_trackbar")
{
    if (!src.empty())
    {
        seulResize(320,240);
        seulMakeHSVTrackbar();
        seulSetHSVTrackbar();
        seulDetectHSVColor();
        seulShowTrackbar();
        seulHoughLinseP();
        seulColoring();
    }
    else
    {
        std::cout << "frame is empty\n";
    }
}

void SeulLaneDetection::seulResize(int cols_, int rows_)
{
    cv::resize(src, src, cv::Size(cols_, rows_), 0, 0, CV_INTER_AREA);
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

void SeulLaneDetection::seulHoughLinseP(){
    float ladian;
    int degree;
    cv::Mat hough_result = src.clone();
    cv::Mat hough_yellow_img = dst_y.clone();
    cv::Mat hough_white_img = dst_w.clone();
    std::vector<cv::Vec4i> yellow_lines, white_lines;
    std::vector<cv::Vec4i>::iterator yellow_it, white_it;
    cv::HoughLinesP(hough_yellow_img,yellow_lines,1,CV_PI/180.0,80,100);
    if(!yellow_lines.empty()){
        yellow_it = yellow_lines.end() - 1;
        ladian = atan2f((*yellow_it)[3]-(*yellow_it)[1], (*yellow_it)[2]-(*yellow_it)[0]);
        degree = ladian * 180/CV_PI;
        if(abs(degree)>=20 && abs(degree)<=80){
            cv::line(hough_result,
                    cv::Point((*yellow_it)[0], (*yellow_it)[1]), 
                    cv::Point((*yellow_it)[2],(*yellow_it)[3]),
                    cv::Scalar(255,200,20),3,0);
        }
        
        cv::imshow("left", hough_result);
    }
}

void SeulLaneDetection::seulColoring(){
    cv::Mat color_img = src.clone();
    cv::Mat lane_img = dst_w | dst_y;
    for(int y = color_img.rows-1; y >= 0; y--){
        uchar *lane_data = lane_img.ptr<uchar>(y);
        uchar *color_data = color_img.ptr<uchar>(y);
        for(int x = 0; x < color_img.cols; x++){
            if(lane_data[x] != (uchar)0){
                color_data[x*color_img.channels()+1] = 25;
            }
        }
    }
    cv::imshow(group_name + "_coloring", color_img);
}