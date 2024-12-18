#pragma once

namespace CultyEngine::Video
{
    class VideoPlayer
    {
    public:
        VideoPlayer();
        ~VideoPlayer();

        bool Initialize(const std::string& filePath);
        void Update();
        void Shutdown();

        // Retrieve the raw frame data (in RGB format)
        const uint8_t* GetFrameData() const { return mFrameRGBBuffer; }

        // Get frame dimensions
        int GetFrameWidth() const { return mCodecContext->width; }
        int GetFrameHeight() const { return mCodecContext->height; }

    private:
        AVFormatContext* mFormatContext = nullptr;
        AVCodecContext* mCodecContext = nullptr;
        AVFrame* mFrame = nullptr;
        AVPacket* mPacket = nullptr;

        SwsContext* mSwsContext = nullptr;
        uint8_t* mFrameRGBBuffer = nullptr;

        int mVideoStreamIndex = -1;
    };
}