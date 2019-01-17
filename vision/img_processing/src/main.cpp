#include <img_processing/seul_calibration/seul_calibration.h>

#include <img_processing/seul_lane_detection/seul_lane_detection.h>

#include <ros/ros.h>
#include <ros/package.h>
#include <iostream>

#include <sstream>


#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//카메라 번호를 넣으면 ..? 이미지 객체로 바뀌어야 함..?
//1. 그룹노드를 정해서 각각 이 클래스를 적용시키면 각각 이미지 객체를 반환한다. 이작업을 이미지가 필요한
//교통표지 인식 모듈에서 직접 그룹별로 이미지 객체를 바로 변환해 사용하도록 한다. 
//이경우 이 클래스에서 카메라랑 연결해서 이미지 받아오고, opencv객체에 저장해서 그걸 다시 ros이미지 객체에 저장해줌

//2. mat이미지를 넣으면 이미지 객체로 변환한다. 
//교통표지 인식 모듈에서 카메라에 직접 연결함(그룹별로) 그래서 클래스멤버 변수로 저장되는 ros이미지 객체 사용하도록


//3. mat 이미지를 넣으면 이미지객체로 변환
//단 이경우 클래스 하나가 더있음. 비디오데이터를 mat으로 바꿔주는 클래스.. 근데 굳이 그럴 필요가 잇는가.
//이경우 비디오 데이터 클래스는 카메라 번호를 인자로 받고 그거에 맞게 mat을 반환하겟지.
//그럼 교통표지 인식 모듈은 그룹별로 번호를 다르게 넘기고, mat을 반환하면 됨.
//문제가 있다면... 

//카메라가 여러대. -> 여기서 초기화된 카메라의 이미지 번호들은 다른 모듈들에서 항상 똑같이 들어와야함.
//모듈마다 초기화하면..ㅇ 나됨...
//메인에서 런치파일로 모듈함수들을 호출할 때 이미 세팅된 이미지를 갖고 파일들을 쓰면 되겟다.
// 그럼 교통표지모듈 자체는 로스 의존적일 필요는 없겠다..
// 하지만 우리 플젝 같은 경우 차선 정보를 라이다로 넘길거지만
//그 메인에서 모듈함수를 썼을 때 딱 이진화된 차선의 fitting정보만 읽을 수 있으면 걔가 정리해서 보내면 될듯..


//비전에서
//1. 교통 정보 인식을 바탕으로 한 정보 발행 패키지(메인)
//- 전처리(캘리브레이션, ros img obj화-> 메인에 넘길떄만..쓰면 되지 않을까)
//- 교통표지 인식 모듈 (캘리브레이션 된 이미지만 받아옴)
//- 
    // public:
    //     SeulImgMsg(){
    //         //it을 초기화해서
    //         //cvimg로 이미지 객체 변환.
    //     }
    //     void CaptureImg(){
    //         //videocapture 변수를 이용해서 frame객체를 저장.
    //         //영상을 불러올 카메라 넘버를 인자로 필요로하고,
    //         //영상을 연속적으로 저장한걸 mat으로 저장.(연속성의 구현은 이함수 바깥..)
    //         //
    //     }

    // private:

void seulInitMainParam(ros::NodeHandle& nh){
   
}
int main(int argc, char** argv){
    
    //img_processing_node initialization
    ros::init(argc, argv, "img_processing_node");
    ros::NodeHandle nh;

    //image calibration
    std::string cali_yaml_path = ros::package::getPath("img_processing")+"/src/camera_main.yaml";
    SeulCaliMatrix cali_img(cali_yaml_path);

    const std::string& group_name = argv[1];
    int cam_num;
    std::istringstream(argv[2]) >> cam_num;
    
    std::cout<<"cam_num : "<<cam_num<<std::endl;    
    std::cout<<"cam_num : "<<group_name<<std::endl;
    cv::VideoCapture cap;
    cv::Mat frame;
    cam_num-=1;
    if(cap.open(cam_num)){
        ros::Rate loop_rate(30);
        while(nh.ok()){
            cap >> frame;
            //------------------------------------------
            cali_img.MyReMap(frame);
            SeulLaneDetection test_lane(frame, nh,group_name);
            cv::imshow("rel img",frame);
            int ckey = cv::waitKey(1);
            if(ckey == 27)break;
            loop_rate.sleep();
        }
    }
    else{
        // CV_ERROR("current camera number("+cam_num+") is not correct",cap.open());
    }
    
    
    return 0;
}
