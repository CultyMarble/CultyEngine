#include <CultyEngine/Inc/CultyEngine.h>

class MainState : public CultyEngine::ApplicationState
{
public:
    void Initialize()
    {
        LOG("MAIN STATE INITIALIZED!");
        CultyEngine::Graphics::GraphicsSystem::Get()->SetClearColor(CultyEngine::Colors::Red);
        mLifeTime = 2.0f;
    }

    void Terminate()
    {
        LOG("MAIN STATE TERMINATED!");
    }

    void Update(float deltaTime)
    {
        mLifeTime -= deltaTime;
        if (mLifeTime <= 0.0f)
        {
            CultyEngine::Application& myApplication = CultyEngine::MainApplication();
            myApplication.ChangeState("GameState");
        }
    }

private:
    float mLifeTime = {};
};

class GameState : public CultyEngine::ApplicationState
{
public:
    void Initialize()
    {
        LOG("GAME STATE INITIALIZED!");
        CultyEngine::Graphics::GraphicsSystem::Get()->SetClearColor(CultyEngine::Colors::Blue);
        mLifeTime = 2.0f;
    }

    void Terminate()
    {
        LOG("GAME STATE TERMINATED!");
    }

    void Update(float deltaTime)
    {
        mLifeTime -= deltaTime;
        if (mLifeTime <= 0.0f)
        {
            CultyEngine::Application& myApplication = CultyEngine::MainApplication();
            myApplication.ChangeState("MainState");
        }
    }

private:
    float mLifeTime = {};
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    // CONFIG
    CultyEngine::ApplicationConfig myAppConfig;
    myAppConfig.applicationName = L"HelloWindow";
    myAppConfig.winWidth = 1280;
    myAppConfig.winHeight = 720;

    // APPLICATION
    CultyEngine::Application& myApplication = CultyEngine::MainApplication();
    myApplication.AddState<MainState>("MainState");
    myApplication.AddState<GameState>("GameState");

    // RUNTIME
    myApplication.Run(myAppConfig);

    return EXIT_SUCCESS;
}