#include <CultyEngine/Inc/CultyEngine.h>
#include "GameState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    // CONFIG
    CultyEngine::ApplicationConfig myAppConfig;
    myAppConfig.applicationName = L"Hello Mesh Builder";

    // APPLICATION
    CultyEngine::Application& myApplication = CultyEngine::MainApplication();
    myApplication.AddState<StateSpherePX>("StateSpherePX");
    myApplication.AddState<StateCube>("StateCube");
    myApplication.AddState<StateRect>("StateRect");
    myApplication.AddState<StateVertPlane>("StateVertPlane");
    myApplication.AddState<StateHorzPlane>("StateHorzPlane");
    myApplication.AddState<StateSphere>("StateSphere");
    myApplication.AddState<StateCylinder>("StateCylinder");
    myApplication.AddState<StateSkyBox>("StateSkyBox");
    myApplication.AddState<StateSkySphere>("StateSkySphere");

    // RUNTIME
    myApplication.Run(myAppConfig);

    return EXIT_SUCCESS;
}