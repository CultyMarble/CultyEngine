#include "Precompiled.h"
#include "Window.h"

LRESULT CALLBACK WindowMessageHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    break;
    default:
        break;
    }

    return DefWindowProc(handle, message, wParam, lParam);
}

namespace CultyEngine
{
    void Window::Initialize(HINSTANCE instance, const std::wstring& applicationName, uint32_t width, uint32_t height)
    {
        mInstance = instance;
        mApplicationName = applicationName;

        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowMessageHandler;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = instance;
        wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = mApplicationName.c_str();
        wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

        RegisterClassEx(&wcex);

        mScreenRect = { 0, 0, (LONG)width, (LONG)height };
        AdjustWindowRect(&mScreenRect, WS_OVERLAPPEDWINDOW, FALSE);

        const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        const int winWidth = std::min(static_cast<int>(mScreenRect.right - mScreenRect.left), screenWidth);
        const int winHeight = std::min(static_cast<int>(mScreenRect.bottom - mScreenRect.top), screenHeight);
        const int left = (screenWidth - winWidth) / 2;
        const int top = (screenHeight - winHeight) / 2;

        mScreenRect.left = left;
        mScreenRect.top = top;

        mWindow = CreateWindow(
            mApplicationName.c_str(),
            mApplicationName.c_str(),
            WS_OVERLAPPEDWINDOW,
            left, top,
            winWidth, winHeight,
            nullptr, nullptr,
            instance, nullptr
        );

        ShowWindow(mWindow, SW_SHOWNORMAL);
        SetCursorPos(screenWidth / 2, screenHeight / 2);

        mIsActive = (mWindow != nullptr);
    }

    void Window::Terminate()
    {
        DestroyWindow(mWindow);
        UnregisterClass(mApplicationName.c_str(), mInstance);

        mWindow = nullptr;
        mInstance = nullptr;
        mIsActive = false;
    }

    void Window::ProcessMessage()
    {
        MSG msg{};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (WM_QUIT == msg.message)
                mIsActive = false;
        }
    }
}