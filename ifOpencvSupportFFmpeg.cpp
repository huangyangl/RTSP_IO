#include <cstdio>
#include <opencv2/opencv.hpp>  
#include <iostream> 
int main(int argc, char **argv){
    std::string info = cv::getBuildInformation();  
    std::cout << info << std::endl;  
  
    // 你可以搜索字符串来查找"FFmpeg"  
    if (info.find("FFmpeg") != std::string::npos || info.find("ffmpeg") != std::string::npos || info.find("FFMPEG") != std::string::npos) {  
        std::cout << "FFmpeg support is enabled." << std::endl;  
    } else {  
        std::cout << "FFmpeg support is not enabled." << std::endl;  
    }  
  
    return 0; 
}