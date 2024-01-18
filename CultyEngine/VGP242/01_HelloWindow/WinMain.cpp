#include <CultyEngine/Inc/CultyEngine.h>

class MainState : public CultyEngine::ApplicationState
{
public:
    void Initialize()
    {
        LOG("MAIN STATE INITIALIZED!");
        mLifeTime = 2.0f;
    }

    void Terminate()
    {
        LOG("MAIN STATE TERMINATED!");
    }

    void Update(float deltaTime)
    {
        mLifeTime -= deltaTime;
        if ()
    }
};

int WINAPI WinMain(HINSTANCE istance, HINSTANCE, LPSTR, int)
{
    CultyEngine::Application myApplication;
    CultyEngine::ApplicationConfig myAppConfig;
    myAppConfig.applicationName = L"HelloWindow";

    myApplication.Run(myAppConfig);

    return EXIT_SUCCESS;
}