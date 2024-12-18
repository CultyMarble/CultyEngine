#include <CultyEngine/Inc/CultyEngine.h>
#include "GameState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    // CONFIG
    CultyEngine::ApplicationConfig myAppConfig;
    myAppConfig.applicationName = L"Hello Diva";
    myAppConfig.winWidth = 1280.0f;
    myAppConfig.winHeight = 720.0f;

    // APPLICATION
    CultyEngine::Application& myApplication = CultyEngine::MainApplication();
    myApplication.AddState<GameState>("GameState");

    // RUNTIME
    myApplication.Run(myAppConfig);

    return EXIT_SUCCESS;
}