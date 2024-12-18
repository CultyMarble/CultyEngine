#pragma once
#include "InputTypes.h"
#include <dinput.h>

namespace CultyEngine::Input
{
    class InputSystem final
    {
    public:
        static void StaticInitialize(HWND window);
        static void StaticTerminate();
        static InputSystem* Get();

    public:
        InputSystem() = default;
        ~InputSystem();

        InputSystem(const InputSystem&) = delete;
        InputSystem& operator=(const InputSystem&) = delete;

        void Initialize(HWND window);
        void Terminate();
        void Update();

        bool IsKeyDown(KeyCode key) const;
        bool IsKeyPressed(KeyCode key) const;

        bool IsMouseDown(MouseButton button) const;
        bool IsMousePressed(MouseButton button) const;

        int GetMouseMoveX() const;
        int GetMouseMoveY() const;
        float GetMouseMoveZ() const;

        int GetMouseScreenX() const;
        int GetMouseScreenY() const;

        bool IsMouseLeftEdge() const;
        bool IsMouseRightEdge() const;
        bool IsMouseTopEdge() const;
        bool IsMouseBottomEdge() const;

        void ShowSystemCursor(bool show);
        void SetMouseClipToWindow(bool clip);
        bool IsMouseClipToWindow() const;

        // DirectInput Functions
        bool IsControllerButtonDown(int button) const;
        bool IsControllerButtonHeld(int button) const;
        bool IsControllerButtonUp(int button) const;

    private:
        void DirectInputInitialize();
        void DirectInputUpdate();
        void DirectInputCleanup();

        static LRESULT CALLBACK InputSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
        static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* deviceInstance, VOID* context);

        HWND mWindow = nullptr;

        bool mCurrKeys[512]{};
        bool mPrevKeys[512]{};
        bool mPressedKeys[512]{};

        bool mClipMouseToWindow = false;

        int mCurrMouseX = -1;
        int mCurrMouseY = -1;
        int mPrevMouseX = -1;
        int mPrevMouseY = -1;
        int mMouseMoveX = 0;
        int mMouseMoveY = 0;

        float mMouseWheel = 0.0f;

        bool mCurrMouseButtons[3]{};
        bool mPrevMouseButtons[3]{};
        bool mPressedMouseButtons[3]{};

        bool mMouseLeftEdge = false;
        bool mMouseRightEdge = false;
        bool mMouseTopEdge = false;
        bool mMouseBottomEdge = false;

        bool mInitialized = false;

        // DirectInput
        IDirectInput8* mDirectInput = nullptr;
        IDirectInputDevice8* mJoystick = nullptr;
        DIJOYSTATE mCurrState{};
        DIJOYSTATE mPrevState{};
    };
}