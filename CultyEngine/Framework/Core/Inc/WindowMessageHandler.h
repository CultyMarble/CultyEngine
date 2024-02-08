#pragma once

namespace CultyEngine::Core
{
	class WindowMessageHandler
	{
	public:
		using CallBack = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

		void Hook(HWND window, CallBack callback);
		void UnHook();

		LRESULT ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND mWindow = nullptr;
		CallBack mPreviousCallback = nullptr;
	};
}