#pragma once
#include <wtypes.h>

namespace CultyEngine::Graphics::DebugUI
{
	enum class Theme
	{
		Classic,
		Dark,
		FlashBang,
	};

	void StaticInitialize(HWND window, bool docking = false, bool multiViewPort = false);
	void StaticTerminate();

	void SetTheme(Theme theme);

	void BeginRender();
	void EndRender();
}