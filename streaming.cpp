extern "C" {  
#include <libavformat/avformat.h>  
#include <libavutil/timestamp.h>  
}  
#include <iostream>  
#include <thread>  
#include <mutex>  
#include <condition_variable>  
#include <queue>  
using namespace std;
// 读取并转发数据包 
void forward_stream(AVFormatContext* ifmt_ctx, AVFormatContext* ofmt_ctx) {  
    AVPacket *pkt = av_packet_alloc();
    // 循环读取输入流中的数据包  
    while (av_read_frame(ifmt_ctx, pkt) >= 0) {  
        // 检查是否需要调整时间戳（这里作为示例，我们简单地将其加上一个固定的延迟）  
        // 注意：实际应用中，你可能需要根据流的具体情况和需要来调整时间戳  
        //int64_t delay = AV_TIME_BASE; // 假设延迟为1秒（AV_TIME_BASE是FFmpeg中定义的时间基准，通常为1,000,000）  
        //pkt->pts = av_rescale_q_rnd(pkt->pts + delay, ifmt_ctx->streams[pkt->stream_index]->time_base, ofmt_ctx->streams[pkt->stream_index]->time_base, AV_ROUND_PASS_MINMAX);  
        //pkt->dts = av_rescale_q_rnd(pkt->dts + delay, ifmt_ctx->streams[pkt->stream_index]->time_base, ofmt_ctx->streams[pkt->stream_index]->time_base, AV_ROUND_PASS_MINMAX);  
        //pkt->dts = pkt->dts + delay; // some_delay是你想要添加的延迟  
        //pkt->pts = pkt->pts + delay; // 同上
        // 将数据包写入输出流  
        // 注意：这里假设输出流的stream_index与输入流的stream_index相匹配，实际情况可能需要更复杂的映射 
        if (av_interleaved_write_frame(ofmt_ctx, pkt) < 0) {  
            // 错误处理  
            fprintf(stderr, "Error muxing packet\n");  
            continue;  
        }  
        // 释放数据包  
        av_packet_unref(pkt);  
    }
    // 写入文件尾  
    av_write_trailer(ofmt_ctx);
    // 释放pkt
    av_packet_free(&pkt);
} 
  
int main(int argc, char **argv) {  
    char* _input_url = "rtsp://192.168.144.25:8554/main.264";
    char* _output_url = "rtmp://127.0.0.1:554/live/a8";
    if (argc >= 2) {
      for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-i" && i + 1 < argc) {
          _input_url = argv[i + 1];
          i++;
        }
        else if (std::string(argv[i]) == "-o" && i + 1 < argc) {
          _output_url = argv[i + 1];
          i++;
        }
        else if (std::string(argv[i]) == "-h") {
            std::cout << "\nUsage:\n"
            << "  " << argv[0]
            << " [-i <input_url>] [-o <output_url>]\n"
            << std::endl
            << "Description:\n"
            << "  -i <input_url>\n"
            << "     eg: -i rtsp://192.168.144.25:8554/main.264\n"
            << "  -o <output_url>\n"
            << "     eg: -o rtmp://127.0.0.1:554/live/test\n"
            << "     eg: -o rtsp://127.0.0.1:554/live/test\n"
            << "  -h\n"
            << "      Print help message.\n"
            << std::endl;
            return 0;
        }
        else {
            std::cout << "Error : unknown parameter " << argv[i] << std::endl<< "See " << argv[0] << " -h" << std::endl;
            return 0;
        }
      }
    }  
    const char* input_url = _input_url;
    const char* output_url = _output_url;
   // 初始化FFmpeg库
    av_register_all();
    AVFormatContext* ifmt_ctx = nullptr;
    AVFormatContext* ofmt_ctx = nullptr;
    try{
        // 打开输入流 
        if (avformat_open_input(&ifmt_ctx, input_url, nullptr, nullptr) != 0) {  
            std::cerr << "Failed to open input stream\n";  
            return 1;  
        }
        // 查找流信息 
        if (avformat_find_stream_info(ifmt_ctx, nullptr) < 0) {  
            std::cerr << "Failed to retrieve input stream information\n";  
            return 1;  
        }
        // 查找输出格式  
        //AVOutputFormat* out_fmt = av_guess_format("rtsp", NULL, NULL); 
        // 分配输出上下文
        std::string output_url_str(_output_url); 
        if(output_url_str.find("rtmp")!= std::string::npos)
            avformat_alloc_output_context2(&ofmt_ctx, nullptr, "flv", output_url);
        else if(output_url_str.find("rtsp")!= std::string::npos)
            avformat_alloc_output_context2(&ofmt_ctx, nullptr, "rtsp", output_url);
        else{
            std::cout << "output_url invalid " << std::endl<< "See " << argv[0] << " -h" << std::endl; 
            return -1; 
        } 
        if (!ofmt_ctx) {
            std::cerr << "Failed to allocate output context\n";
            return -1;  
        }
        // 复制流（音频流、视频流）
        for (unsigned int i = 0; i < ifmt_ctx->nb_streams; i++) {
            AVCodecParameters* codecpar = ifmt_ctx->streams[i]->codecpar;
            AVMediaType type = codecpar->codec_type;
            string typeStr = type == AVMEDIA_TYPE_VIDEO? "Video" : type == AVMEDIA_TYPE_AUDIO? "Audio" : "others";
            std::cerr << "i: " <<i <<";  input stream type: " << typeStr << endl;
            AVStream* in_stream = ifmt_ctx->streams[i];
            AVStream* out_stream = avformat_new_stream(ofmt_ctx, nullptr);//为ofmt_ctx创建一个流（音频、视频）
            if (!out_stream) {
                std::cerr << "Failed allocating output stream\n";
                return -1;
            }
            // 复制输入流的编解码器参数
            if (avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar) < 0) {
                std::cerr << "Failed to copy codec parameters\n";
                return -1;
            }
            // 设置时间基准
            out_stream->time_base = in_stream->time_base; 
        }
        // 查看输出流的编码器类型
        for (unsigned int i = 0; i < ofmt_ctx->nb_streams; i++) {
            AVCodecParameters* codecpar = ofmt_ctx->streams[i]->codecpar;
            AVMediaType type = codecpar->codec_type;
            string typeStr = type == AVMEDIA_TYPE_VIDEO? "Video" : type == AVMEDIA_TYPE_AUDIO? "Audio" : "others";
            std::cerr << "i: " <<i <<";  output stream type: " << typeStr << ";  codec_id: " << ofmt_ctx->streams[i]->codecpar->codec_id <<endl; 
        }

        // 打开输出URL
        if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
            if (avio_open(&ofmt_ctx->pb, output_url, AVIO_FLAG_WRITE) < 0) {
                std::cerr << "Could not open output URL\n";  
                return -1; 
            }  
        }
        // 写入文件头
        if (avformat_write_header(ofmt_ctx, nullptr) < 0) {  
            std::cerr << "Error occurred when opening output URL\n";
            return 1;  
        }
        // 转发  
        forward_stream(ifmt_ctx, ofmt_ctx);//这里有一个死循环
        // 输入流异常会导致forward_stream退出循环，执行下面的代码
        // 关闭文件、清理上下文
        avio_close(ofmt_ctx->pb);
        avformat_free_context(ofmt_ctx);
        avformat_close_input(&ifmt_ctx);
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "catch an Error:"<< '\n';
        std::cerr << e.what() << '\n';
        // 关闭文件、清理上下文
        avio_close(ofmt_ctx->pb);
        avformat_free_context(ofmt_ctx);
        avformat_close_input(&ifmt_ctx);
        return -1;
    }
    return -1;

}