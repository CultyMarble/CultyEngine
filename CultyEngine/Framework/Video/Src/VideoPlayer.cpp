#include "Precompiled.h"
#include "VideoPlayer.h"

#include <iostream>
#include <filesystem>
#include <cstdio> // For fopen, fwrite, fclose

namespace fs = std::filesystem;
using namespace CultyEngine::Video;

bool VideoPlayer::ExtractFrames(const std::string& videoPath, const std::string& outputFolder)
{
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* codecContext = nullptr;
    const AVCodec* codec = nullptr;
    AVFrame* frame = nullptr;
    AVPacket* packet = nullptr;
    SwsContext* swsContext = nullptr;

    int videoStreamIndex = -1;

    // Open the video file
    if (avformat_open_input(&formatContext, videoPath.c_str(), nullptr, nullptr) != 0)
    {
        std::cerr << "Failed to open video file: " << videoPath << std::endl;
        return false;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(formatContext, nullptr) < 0)
    {
        std::cerr << "Failed to retrieve stream info." << std::endl;
        return false;
    }

    // Find the video stream
    for (unsigned int i = 0; i < formatContext->nb_streams; ++i)
    {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1)
    {
        std::cerr << "No video stream found in file: " << videoPath << std::endl;
        return false;
    }

    // Set up codec context
    AVCodecParameters* codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec)
    {
        std::cerr << "Failed to find codec." << std::endl;
        return false;
    }

    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext || avcodec_parameters_to_context(codecContext, codecParams) < 0)
    {
        std::cerr << "Failed to allocate codec context." << std::endl;
        return false;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0)
    {
        std::cerr << "Failed to open codec." << std::endl;
        return false;
    }

    // Allocate structures
    frame = av_frame_alloc();
    packet = av_packet_alloc();
    swsContext = sws_getContext(
        codecContext->width, codecContext->height, codecContext->pix_fmt,
        codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (!frame || !packet || !swsContext)
    {
        std::cerr << "Failed to allocate FFmpeg resources." << std::endl;
        return false;
    }

    // Create output directory if it does not exist
    fs::create_directories(outputFolder);

    // Frame extraction loop
    int frameIndex = 0;
    while (av_read_frame(formatContext, packet) >= 0)
    {
        if (packet->stream_index == videoStreamIndex)
        {
            if (avcodec_send_packet(codecContext, packet) == 0)
            {
                while (avcodec_receive_frame(codecContext, frame) == 0)
                {
                    SaveFrameAsPpm(frame, codecContext->width, codecContext->height, frameIndex++, outputFolder);
                }
            }
        }
        av_packet_unref(packet);
    }

    // Cleanup
    av_packet_free(&packet);
    av_frame_free(&frame);
    sws_freeContext(swsContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);

    std::cout << "All frames extracted successfully!" << std::endl;
    return true;
}

bool VideoPlayer::SaveFrameAsPpm(AVFrame* frame, int width, int height, int frameIndex, const std::string& outputFolder)
{
    std::string filename = outputFolder + "/frame_" + std::to_string(frameIndex) + ".ppm";

    FILE* file = nullptr;
    errno_t err = fopen_s(&file, filename.c_str(), "wb");

    if (!file)
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    // Write the PPM header
    fprintf(file, "P6\n%d %d\n255\n", width, height);

    // Convert the frame to RGB format
    SwsContext* swsContext = sws_getContext(
        width, height, (AVPixelFormat)frame->format,
        width, height, AV_PIX_FMT_RGB24,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    uint8_t* rgbBuffer = new uint8_t[width * height * 3];
    uint8_t* dest[4] = { rgbBuffer, nullptr, nullptr, nullptr };
    int destLinesize[4] = { 3 * width, 0, 0, 0 };

    sws_scale(swsContext, frame->data, frame->linesize, 0, height, dest, destLinesize);

    // Write pixel data to the file
    fwrite(rgbBuffer, 1, width * height * 3, file);

    // Cleanup
    delete[] rgbBuffer;
    sws_freeContext(swsContext);
    fclose(file);

    std::cout << "Saved frame: " << filename << std::endl;
    return true;
}