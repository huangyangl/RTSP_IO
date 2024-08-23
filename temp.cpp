#include <cstdio>
#include <opencv2/opencv.hpp>  
#include <iostream> 
int main(int argc, char **argv){    
    if (argc != 2) {  
        std::cerr << "Usage: " << argv[0] << " <RTSP_URL>" << std::endl;  
        return -1;  
    }  
    // 创建一个VideoCapture对象来打开视频流  
    cv::VideoCapture cap(argv[1],cv::VideoCaptureAPIs::CAP_FFMPEG);  
    // 检查视频是否成功打开  
    if (!cap.isOpened()) {  
        std::cerr << "Error opening video stream or file" << std::endl;  
        return -1;  
    }
    //推流
	std::string rtsp_server_url = "rtsp://127.0.0.1:554/live/0";
	std::stringstream command;
	command << "ffmpeg ";
	// inputfile options
	command << "-y "  // overwrite output files
	<< "-an " // disable audio
	<< "-f rawvideo " // force format to rawvideo
	<< "-vcodec rawvideo "  // force video rawvideo ('copy' to copy stream)
	<< "-pix_fmt bgr24 "  // set pixel format to bgr24
	<< "-s 640x480 "  // set frame size (WxH or abbreviation)
	<< "-r 30 "; // set frame rate (Hz value, fraction or abbreviation)
	command << "-i - ";
	// outputfile options
	command 
	<< "-c:v libx264 "  // Hyper fast Audio and Video encoder
	<< "-pix_fmt yuv420p "  // set pixel format to yuv420p
	<< "-tune:v zerolatency "
	<< "-preset ultrafast " // set the libx264 encoding preset to ultrafast
	<< "-f rtsp " // force format to flv for rtmp, rtsp for rtsp
	<< rtsp_server_url;
	FILE *fp = nullptr;
    try
    {
        // 创建一个窗口来显示视频  
        cv::namedWindow("RTSP Stream", cv::WINDOW_AUTOSIZE);  
        cv::Mat frame;
        // 打开管道
        fp = popen(command.str().c_str(), "w");
        if (fp != nullptr) 
            std::cout << "fp open success!" << std::endl;
        else 
        {
            std::cout << "fp open fail!" << std::endl;
            pclose(fp);
        }
        while (true) {  
            // 读取一帧  
            if (!cap.read(frame)) {  
                std::cerr << "No frame" << std::endl;  
                continue;  
            }
            if(frame.empty()) continue;
            //推流
			fwrite(frame.data, sizeof(char), frame.total() * frame.elemSize(), fp); 
            // 显示帧  
            //cv::imshow("RTSP Stream", frame);  
            // 按 'q' 退出循环  
            if (cv::waitKey(1) == 'q') {  
                break;  
            }  
        }  
        // 释放对象并关闭所有窗口  
        pclose(fp);
        cap.release();
        cv::destroyAllWindows();
        return 0; 
    }
    catch(const std::exception& e)
    {
        std::cerr << "catch a err:" << std::endl; 
        std::cerr << e.what() << '\n';
        // 释放对象并关闭所有窗口
        pclose(fp);
        cap.release();
        cv::destroyAllWindows();
    }
    
}


extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
}
 
int main() {
    AVFormatContext *format_ctx = NULL;
    AVStream *video_stream = NULL;
    AVCodec *codec;
    AVCodecContext *codec_ctx = NULL;
    int ret;
 
    // 1. 分配输出上下文并设置输出格式为RTSP
    avformat_alloc_output_context2(&format_ctx, NULL, "rtsp", "rtsp://your_rtsp_url");
    if (!format_ctx) {
        ret = AVERROR_UNKNOWN;
        goto end;
    }
 
    // 2. 查找编码器
    codec = avcodec_find_encoder(AV_CODEC_ID_H264); // 假设使用H264编码
    if (!codec) {
        ret = AVERROR_UNKNOWN;
        goto end;
    }
 
    // 3. 创建视频流
    video_stream = avformat_new_stream(format_ctx, codec);
    if (!video_stream) {
        ret = AVERROR_UNKNOWN;
        goto end;
    }
    codec_ctx = video_stream->codec;
    codec_ctx->codec_id = AV_CODEC_ID_H264;
    codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    // 设置编码器选项，例如分辨率、帧率等
    codec_ctx->width = 640;
    codec_ctx->height = 480;
    codec_ctx->time_base = (AVRational){1, 25}; // 假设帧率为25
    // 设置输出编码器为H264
    // ... 设置其他编码器参数
 
    // 4. 打开编码器
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        ret = AVERROR_UNKNOWN;
        goto end;
    }
 
    // 5. 写入文件头
    if (!(format_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&format_ctx->pb, "rtsp://your_rtsp_url", AVIO_FLAG_WRITE);
        if (ret < 0) {
            goto end;
        }
    }
    ret = avformat_write_header(format_ctx, NULL);
    if (ret < 0) {
        goto end;
    }
 
    // 6. 编码视频帧并发送至RTSP服务器
    // ... 编码和发送视频帧的代码
 
    // 7. 写入文件尾
end:
    av_write_trailer(format_ctx);
 
    // 关闭资源
    if (format_ctx && !(format_ctx->oformat->flags & AVFMT_NOFILE)) {
        avio_close(format_ctx->pb);
    }
    avformat_free_context(format_ctx);
 
    return ret < 0;
}


extern "C" {  
#include <libavformat/avformat.h>  
#include <libavutil/timestamp.h>  
}  
  
// 假设已经初始化了FFmpeg库，并且打开了输入和输出流  
// AVFormatContext* ifmt_ctx; // 输入流上下文  
// AVFormatContext* ofmt_ctx; // 输出流上下文  
  
// 读取并转发数据包的函数  
void forward_stream(AVFormatContext* ifmt_ctx, AVFormatContext* ofmt_ctx) {  
    AVPacket pkt;  
    int ret;  
  
    // 循环读取输入流中的数据包  
    while ((ret = av_read_frame(ifmt_ctx, &pkt)) >= 0) {  
        // 检查是否需要调整时间戳（这里作为示例，我们简单地将其加上一个固定的延迟）  
        // 注意：实际应用中，你可能需要根据流的具体情况和需要来调整时间戳  
        int64_t delay = AV_TIME_BASE; // 假设延迟为1秒（AV_TIME_BASE是FFmpeg中定义的时间基准，通常为1,000,000）  
        pkt.pts = av_rescale_q_rnd(pkt.pts + delay, ifmt_ctx->streams[pkt.stream_index]->time_base, ofmt_ctx->streams[pkt.stream_index]->time_base, AV_ROUND_PASS_MINMAX);  
        pkt.dts = av_rescale_q_rnd(pkt.dts + delay, ifmt_ctx->streams[pkt.stream_index]->time_base, ofmt_ctx->streams[pkt.stream_index]->time_base, AV_ROUND_PASS_MINMAX);  
  
        // 将数据包写入输出流  
        // 注意：这里假设输出流的stream_index与输入流的stream_index相匹配，实际情况可能需要更复杂的映射  
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);  
        if (ret < 0) {  
            // 错误处理  
            fprintf(stderr, "Error muxing packet\n");  
            break;  
        }  
  
        // 释放数据包  
        av_packet_unref(&pkt);  
    }  
  
    // 检查是否因为到达文件末尾而退出循环  
    if (ret < 0 && ret != AVERROR_EOF) {  
        // 错误处理  
        fprintf(stderr, "Error reading frames from input file\n");  
    }  
  
    // 写入文件尾并关闭输出流  
    av_write_trailer(ofmt_ctx);  
}  
  