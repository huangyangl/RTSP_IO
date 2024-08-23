## 使用文档
# 通过MK32局域网拉取A8mini的视频流：rtsp://192.168.144.25:8554/main.264
# 转发到板载计算机上的流媒体服务器ZLMediaKit
# 转发地址：rtsp://127.0.0.1:554/live/a8
# -i -o 选项可以更改拉流和转发地址
# 视频编解码器是H265
# usage
./rtspStreaming
./rtspStreaming -i rtsp://192.168.144.25:8554/main.264 -o rtsp://127.0.0.1:554/live/a8
# start ZLMediaKit server
cd ZLMediaKit/release/linux/Debug
sudo ./MediaServer -d 
sudo killall -2 MediaServer
ls ZLMediaKit/release/linux/Debug/log
# 查看opencv是否编译了FFmpeg支持
./ifOpencvSupportFFmpeg
