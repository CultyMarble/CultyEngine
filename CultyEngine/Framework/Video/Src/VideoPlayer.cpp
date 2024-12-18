#include "Precompiled.h"
#include "VideoPlayer.h"


using namespace CultyEngine;
using namespace CultyEngine::Video;

VideoPlayer::VideoPlayer()
{

}

VideoPlayer::~VideoPlayer()
{
    Shutdown();
}

bool VideoPlayer::Initialize(const std::string& filePath)
{
    // Open the video file
    if (avformat_open_input(&mFormatContext, filePath.c_str(), nullptr, nullptr) != 0)
    {
        std::cerr << "Failed to open video file: " << filePath << std::endl;
        return false;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(mFormatContext, nullptr) < 0)
    {
        std::cerr << "Failed to retrieve stream info." << std::endl;
        return false;
    }

    // Find the video stream
    for (unsigned int i = 0; i < mFormatContext->nb_streams; i++)
    {
        if (mFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            mVideoStreamIndex = i;
            break;
        }
    }

    if (mVideoStreamIndex == -1)
    {
        std::cerr << "No video stream found." << std::endl;
        return false;
    }

    // Set up codec context
    AVCodecParameters* codecParams = mFormatContext->streams[mVideoStreamIndex]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec)
    {
        std::cerr << "Codec not found." << std::endl;
        return false;
    }

    mCodecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(mCodecContext, codecParams);

    if (avcodec_open2(mCodecContext, codec, nullptr) < 0)
    {
        std::cerr << "Failed to open codec." << std::endl;
        return false;
    }

    // Allocate frames and buffer for RGB conversion
    mFrame = av_frame_alloc();
    mPacket = av_packet_alloc();

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, mCodecContext->width, mCodecContext->height, 1);
    mFrameRGBBuffer = static_cast<uint8_t*>(av_malloc(numBytes * sizeof(uint8_t)));

    mSwsContext = sws_getContext(
        mCodecContext->width, mCodecContext->height, mCodecContext->pix_fmt,
        mCodecContext->width, mCodecContext->height, AV_PIX_FMT_RGB24,
        SWS_BILINEAR, nullptr, nullptr, nullptr
    );

    return true;
}

void VideoPlayer::Update()
{
    if (av_read_frame(mFormatContext, mPacket) >= 0)
    {
        if (mPacket->stream_index == mVideoStreamIndex)
        {
            avcodec_send_packet(mCodecContext, mPacket);
            if (avcodec_receive_frame(mCodecContext, mFrame) == 0)
            {
                // Convert to RGB format
                uint8_t* dest[4] = { mFrameRGBBuffer, nullptr, nullptr, nullptr };
                int destLinesize[4] = { 3 * mCodecContext->width, 0, 0, 0 };

                sws_scale(
                    mSwsContext,
                    mFrame->data, mFrame->linesize,
                    0, mCodecContext->height,
                    dest, destLinesize
                );

                std::cout << "Decoded frame: " << mFrame->pts << std::endl;
            }
        }
        av_packet_unref(mPacket);
    }
}

void VideoPlayer::Shutdown()
{
    if (mFrameRGBBuffer) av_free(mFrameRGBBuffer);
    if (mSwsContext) sws_freeContext(mSwsContext);
    if (mFrame) av_frame_free(&mFrame);
    if (mPacket) av_packet_free(&mPacket);
    if (mCodecContext) avcodec_free_context(&mCodecContext);
    if (mFormatContext) avformat_close_input(&mFormatContext);
}