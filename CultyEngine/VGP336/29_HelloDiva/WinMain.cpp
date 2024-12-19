#include <CultyEngine/Inc/CultyEngine.h>
#include "GameState.h"

/// <summary>
///
/// READ ME: After cloning the project. The first time build will throw a missing dlls file error
///          Please go to CultyEngineRepos\CultyEngine\External\FFmpeg\Keepdll\
///          Copy all the dlls file and paste them to CultyEngineRepos\CultyEngine\x64\Debug\
///          
///          Please always keep at least 1 .ppm file in folders 
///             CultyEngineRepos\CultyEngine\Assets\Videos\ppmFrame
///             CultyEngineRepos\CultyEngine\Assets\Videos\ppmFrames
/// 
///          In GameState::Initialize()
///             To detele all .ppm files
///                 Use CleanFrameFolder("../../Assets/Videos/ppmFrames");
///             to generalte .ppm files from a mp4
///                 Use ExtractFramesFromVideo("../../Assets/Videos/rnc.mp4", "../../Assets/Videos/ppmFrames");
/// 
///          // Commend out CleanFrameFolder and ExtractFramesFromVideo to use
///          LoadPPMFramesAsTextures("../../Assets/Videos/ppmFrame");
/// 
/// </summary>

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