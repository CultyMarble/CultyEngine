#include <CultyEngine/Inc/CultyEngine.h>
#include "GameState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    // CONFIG
    CultyEngine::ApplicationConfig myAppConfig;
    myAppConfig.applicationName = L"Hello Simple Draw";

    // APPLICATION
    CultyEngine::Application& myApplication = CultyEngine::MainApplication();
    myApplication.AddState<StateTransform>("StateTransform");
    myApplication.AddState<StateGroundPlane>("StateGroundPlane");
    myApplication.AddState<StateSphere>("StateSphere");
    myApplication.AddState<StateAABB>("StateAABB");
    myApplication.AddState<StateFilledAABB>("StateFilledAABB");
    myApplication.AddState<StateLines>("StateLines");

    // RUNTIME
    myApplication.Run(myAppConfig);

    return EXIT_SUCCESS;
}