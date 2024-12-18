#pragma once

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <string>

namespace CultyEngine::Video
{
    class VideoPlayer
    {
    public:
        bool ExtractFrames(const std::string& videoPath, const std::string& outputFolder);

    private:
        bool SaveFrameAsPpm(AVFrame* frame, int width, int height, int frameIndex, const std::string& outputFolder);
    };
}
