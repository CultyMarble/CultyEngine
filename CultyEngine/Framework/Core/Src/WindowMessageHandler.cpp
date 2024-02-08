#include "Precompiled.h"
#include "WindowMessageHandler.h"
#include "DebugUtils.h"

using namespace CultyEngine::Core;

void WindowMessageHandler::Hook(HWND window, CallBack callback)
{
	mWindow = window;
	mPreviousCallback = (CallBack)GetWindowLongPtrA(window, GWLP_WNDPROC);
	SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)callback);
}

void WindowMessageHandler::UnHook()
{
	SetWindowLongPtrA(mWindow, GWLP_WNDPROC, (LONG_PTR)mPreviousCallback);
	mWindow = nullptr;
}

LRESULT WindowMessageHandler::ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ASSERT(mPreviousCallback != nullptr, "WindowMessageHandler: No callback is hooked!");
	return CallWindowProcA((WNDPROC)mPreviousCallback, window, message, wParam, lParam);
}