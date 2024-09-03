# 顶层CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(RTSPio VERSION 0.1)
# 寻找OpenCV库  
find_package(OpenCV REQUIRED)  
# 查找FFmpeg库
find_package(PkgConfig REQUIRED)  
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libavutil libswscale) 

# ifOpencvSupportFFmpeg
# 添加可执行文件 
add_executable(ifOpencvSupportFFmpeg)
target_sources(ifOpencvSupportFFmpeg PRIVATE ifOpencvSupportFFmpeg.cpp)
# 添加头文件
target_include_directories(ifOpencvSupportFFmpeg PRIVATE /usr/include/opencv4/)
# 链接OpenCV库  
target_link_libraries(ifOpencvSupportFFmpeg ${OpenCV_LIBS})

# streaming
# 添加可执行文件 
add_executable(streaming)
target_sources(streaming PRIVATE streaming.cpp)
# 添加头文件
target_include_directories(streaming PRIVATE /usr/include/opencv4/)
# 链接OpenCV库  
target_link_libraries(streaming ${FFMPEG_LIBRARIES})
