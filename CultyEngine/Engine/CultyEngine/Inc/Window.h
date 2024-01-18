#pragma once

namespace CultyEngine
{
    class Window
    {
    public:
        void Initialize(HINSTANCE instance, const std::string& applicationName, uint32_t width, uint32_t height);
        void Terminate();

        void ProcessMessage();

        HWND GetWindowHandle() const { return mWindow; }
        bool IsActive() const { return mIsActive; }

    private:
        HINSTANCE mInstance = nullptr;
        HWND mWindow = nullptr;
        RECT mScreenRect{};
        std::wstring mApplicationName;
        bool mIsActive = false;
    };
}