#include "Precompiled.h"
#include "InputSystem.h"

#include <Windows.h>
#include <dinput.h>
#include <cassert>
#include <cstring>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace CultyEngine;
using namespace CultyEngine::Input;

namespace
{
    std::unique_ptr<InputSystem> sInputSystem;
    Core::WindowMessageHandler sWindowMessageHandler;

    void ClipToWindow(HWND window)
    {
        RECT rect;
        GetClientRect(window, &rect);

        POINT ul;
        ul.x = rect.left;
        ul.y = rect.top;

        POINT lr;
        lr.x = rect.right;
        lr.y = rect.bottom;

        MapWindowPoints(window, nullptr, &ul, 1);
        MapWindowPoints(window, nullptr, &lr, 1);

        rect.left = ul.x;
        rect.top = ul.y;

        rect.right = lr.x;
        rect.bottom = lr.y;

        ClipCursor(&rect);
    }
}

LRESULT CALLBACK InputSystem::InputSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (sInputSystem)
    {
        switch (message)
        {
        case WM_ACTIVATEAPP:
        {
            if (wParam == TRUE)
            {
                SetCapture(window);
            }
            else
            {
                sInputSystem->mMouseLeftEdge = false;
                sInputSystem->mMouseRightEdge = false;
                sInputSystem->mMouseTopEdge = false;
                sInputSystem->mMouseBottomEdge = false;
                ReleaseCapture();
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            sInputSystem->mCurrMouseButtons[0] = true;
            break;
        }
        case WM_LBUTTONUP:
        {
            sInputSystem->mCurrMouseButtons[0] = false;
            break;
        }
        case WM_RBUTTONDOWN:
        {
            sInputSystem->mCurrMouseButtons[1] = true;
            break;
        }
        case WM_RBUTTONUP:
        {
            sInputSystem->mCurrMouseButtons[1] = false;
            break;
        }
        case WM_MBUTTONDOWN:
        {
            sInputSystem->mCurrMouseButtons[2] = true;
            break;
        }
        case WM_MBUTTONUP:
        {
            sInputSystem->mCurrMouseButtons[2] = false;
            break;
        }
        case WM_MOUSEWHEEL:
        {
            sInputSystem->mMouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
            break;
        }
        case WM_MOUSEMOVE:
        {
            int mouseX = (signed short)(lParam);
            int mouseY = (signed short)(lParam >> 16);

            sInputSystem->mCurrMouseX = mouseX;
            sInputSystem->mCurrMouseY = mouseY;
            if (sInputSystem->mPrevMouseX == -1)
            {
                sInputSystem->mPrevMouseX = mouseX;
                sInputSystem->mPrevMouseY = mouseY;
            }

            RECT rect;
            GetClientRect(window, &rect);
            sInputSystem->mMouseLeftEdge = mouseX <= rect.left;
            sInputSystem->mMouseRightEdge = mouseX + 1 >= rect.right;
            sInputSystem->mMouseTopEdge = mouseY <= rect.top;
            sInputSystem->mMouseBottomEdge = mouseY + 1 >= rect.bottom;
            break;
        }
        case WM_KEYDOWN:
        {
            if (wParam < 256)
            {
                sInputSystem->mCurrKeys[wParam] = true;
            }
            break;
        }
        case WM_KEYUP:
        {
            if (wParam < 256)
            {
                sInputSystem->mCurrKeys[wParam] = false;
            }
            break;
        }
        }
    }

    return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

BOOL InputSystem::EnumJoysticksCallback(const DIDEVICEINSTANCE* deviceInstance, VOID* context)
{
    auto inputSystem = static_cast<InputSystem*>(context);
    if (SUCCEEDED(inputSystem->mDirectInput->CreateDevice(deviceInstance->guidInstance, &inputSystem->mJoystick, nullptr)))
    {
        inputSystem->mJoystick->SetDataFormat(&c_dfDIJoystick);
        inputSystem->mJoystick->Acquire();
        LOG("DirectInput -- Controller detected: %s", deviceInstance->tszProductName);
        return DIENUM_STOP;
    }
    return DIENUM_CONTINUE;
}

void InputSystem::StaticInitialize(HWND window)
{
    ASSERT(sInputSystem == nullptr, "InputSystem -- System already initialized!");
    sInputSystem = std::make_unique<InputSystem>();
    sInputSystem->Initialize(window);
}

void InputSystem::StaticTerminate()
{
    if (sInputSystem != nullptr)
    {
        sInputSystem->Terminate();
        sInputSystem.reset();
    }
}

InputSystem* InputSystem::Get()
{
    ASSERT(sInputSystem != nullptr, "InputSystem -- No system registered.");
    return sInputSystem.get();
}

InputSystem::~InputSystem()
{
    ASSERT(!mInitialized, "InputSystem -- Terminate() must be called to clean up!");
}

void InputSystem::Initialize(HWND window)
{
    if (mInitialized)
    {
        LOG("InputSystem -- Already initialized.");
        return;
    }

    LOG("InputSystem -- Initializing...");

    mWindow = window;

    sWindowMessageHandler.Hook(window, InputSystemMessageHandler);
    DirectInputInitialize();

    mInitialized = true;

    LOG("InputSystem -- System initialized.");
}

void InputSystem::Terminate()
{
    // Check if we have already terminated the system
    if (!mInitialized)
    {
        LOG("InputSystem -- System already terminated.");
        return;
    }

    LOG("InputSystem -- Terminating...");

    mInitialized = false;

    DirectInputCleanup();
    sWindowMessageHandler.UnHook();

    LOG("InputSystem -- System terminated.");
}

void InputSystem::Update()
{
    ASSERT(mInitialized, "InputSystem -- System not initialized.");

    // Store the previous keyboard state
    for (int i = 0; i < 512; ++i)
    {
        mPressedKeys[i] = !mPrevKeys[i] && mCurrKeys[i];
    }
    memcpy(mPrevKeys, mCurrKeys, sizeof(mCurrKeys));

    // Update mouse movement
    mMouseMoveX = mCurrMouseX - mPrevMouseX;
    mMouseMoveY = mCurrMouseY - mPrevMouseY;
    mPrevMouseX = mCurrMouseX;
    mPrevMouseY = mCurrMouseY;

    // Store the previous mouse state
    for (int i = 0; i < 3; ++i)
    {
        mPressedMouseButtons[i] = !mPrevMouseButtons[i] && mCurrMouseButtons[i];
    }
    memcpy(mPrevMouseButtons, mCurrMouseButtons, sizeof(mCurrMouseButtons));

    // DirectInput Update
    DirectInputUpdate();
}

bool InputSystem::IsKeyDown(KeyCode key) const
{
    return mCurrKeys[(int)key];
}

bool InputSystem::IsKeyPressed(KeyCode key) const
{
    return mPressedKeys[(int)key];
}

bool InputSystem::IsMouseDown(MouseButton button) const
{
    return mCurrMouseButtons[(int)button];
}

bool InputSystem::IsMousePressed(MouseButton button) const
{
    return mPressedMouseButtons[(int)button];
}

int InputSystem::GetMouseMoveX() const
{
    return mMouseMoveX;
}

int InputSystem::GetMouseMoveY() const
{
    return mMouseMoveY;
}

float InputSystem::GetMouseMoveZ() const
{
    return mMouseWheel;
}

int InputSystem::GetMouseScreenX() const
{
    return mCurrMouseX;
}

int InputSystem::GetMouseScreenY() const
{
    return mCurrMouseY;
}

bool InputSystem::IsMouseLeftEdge() const
{
    return mMouseLeftEdge;
}

bool InputSystem::IsMouseRightEdge() const
{
    return mMouseRightEdge;
}

bool InputSystem::IsMouseTopEdge() const
{
    return mMouseTopEdge;
}

bool InputSystem::IsMouseBottomEdge() const
{
    return mMouseBottomEdge;
}

void InputSystem::ShowSystemCursor(bool show)
{
    ShowCursor(show);
}

void InputSystem::SetMouseClipToWindow(bool clip)
{
    mClipMouseToWindow = clip;
}

bool InputSystem::IsMouseClipToWindow() const
{
    return mClipMouseToWindow;
}

bool InputSystem::IsControllerButtonDown(int button) const
{
    return ((mCurrState.rgbButtons[button] & 0x80) != 0) &&
        ((mPrevState.rgbButtons[button] & 0x80) == 0);
}

bool InputSystem::IsControllerButtonHeld(int button) const
{
    return (mCurrState.rgbButtons[button] & 0x80) != 0;
}

bool InputSystem::IsControllerButtonUp(int button) const
{
    return ((mCurrState.rgbButtons[button] & 0x80) == 0) &&
        ((mPrevState.rgbButtons[button] & 0x80) != 0);
}

void InputSystem::DirectInputInitialize()
{
    if (FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, nullptr)))
    {
        LOG("DirectInput -- Initialization failed.");
        return;
    }

    if (FAILED(mDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, this, DIEDFL_ATTACHEDONLY)))
    {
        LOG("DirectInput -- No controllers found.");
    }
}

void InputSystem::DirectInputUpdate()
{
    if (mJoystick)
    {
        mPrevState = mCurrState;

        if (FAILED(mJoystick->Poll()) ||
            FAILED(mJoystick->GetDeviceState(sizeof(DIJOYSTATE), &mCurrState)))
        {
            LOG("DirectInput -- Controller disconnected.");
            mJoystick->Unacquire();
            mJoystick = nullptr;
        }
    }
}

void InputSystem::DirectInputCleanup()
{
    if (mJoystick)
    {
        mJoystick->Unacquire();
        mJoystick->Release();
        mJoystick = nullptr;
    }

    if (mDirectInput)
    {
        mDirectInput->Release();
        mDirectInput = nullptr;
    }
}