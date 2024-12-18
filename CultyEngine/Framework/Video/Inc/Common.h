#pragma once

#include <cassert>
#include <iostream>
#include <string>

extern "C"
{
#include <FFmpeg/include/libavformat/avformat.h>
#include <FFmpeg/include/libavcodec/avcodec.h>
#include <FFmpeg/include/libavutil/avutil.h>
#include <FFmpeg/include/libavutil/frame.h>     // Required for AVFrame
#include <FFmpeg/include/libavutil/imgutils.h>  // For av_image_get_buffer_size
#include <FFmpeg/include/libavutil/pixfmt.h>    // For AV_PIX_FMT_RGB24
#include <FFmpeg/include/libswscale/swscale.h>  // For sws_getContext
}