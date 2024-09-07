#include "Precompiled.h"
#include "Application.h"
#include "ApplicationState.h"
#include "EventManager.h"

using namespace CultyEngine;
using namespace CultyEngine::Core;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;
using namespace CultyEngine::Physics;
using namespace CultyEngine::Audio;

bool gIsMinimized = false;

void Application::Run(const ApplicationConfig& config)
{
    LOG("Application Started: %.3f", TimeUtils::GetTime());

    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        config.applicationName,
        config.winWidth,
        config.winHeight
    );

    ASSERT(myWindow.IsActive(), "Failed to create a Window!");
    auto wHandle = myWindow.GetWindowHandle();
    GraphicsSystem::StaticInitialize(wHandle, false);
    InputSystem::StaticInitialize(wHandle);
    DebugUI::StaticInitialize(wHandle, false, true);
    SimpleDraw::StaticInitialize(config.maxVertexCount);
    TextureManager::StaticInitialize("../../Assets/Images/");
    ModelManager::StaticInitialize();
    AudioSystem::StaticInitialize();
    SoundEffectManager::StaticInitialize("../../Assets/Sounds");
    EventManager::StaticInitialize();

    PhysicsWorld::Settings settings;
    PhysicsWorld::StaticInitialize(settings);

    ASSERT(mCurrentState != nullptr, "Application: need an application state!");
    mCurrentState->Initialize();

    mIsRunning = true;
    while (mIsRunning)
    {
        myWindow.ProcessMessage();
        InputSystem* input = InputSystem::Get();
        input->Update();

        if (myWindow.IsActive() == false || input->IsKeyPressed(KeyCode::ESCAPE))
        {
            Quit();
            break;
        }

        if (gIsMinimized)
        {
            Sleep(100);
            continue;
        }

        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }

        AudioSystem::Get()->Update();

        float deltaTime = TimeUtils::GetDeltaTime();
        if (deltaTime < 0.5f)
        {
            PhysicsWorld::Get()->Update(deltaTime);
            mCurrentState->Update(deltaTime);
        }

        // Render
        GraphicsSystem* gs = GraphicsSystem::Get();
        gs->BeginRender();
            mCurrentState->Render();
            DebugUI::BeginRender();
                mCurrentState->DebugUI();
            DebugUI::EndRender();
        gs->EndRender();
    }

    // Clean Up
    mCurrentState->Terminate();

    EventManager::StaticTerminate();
    SoundEffectManager::StaticTerminate();
    AudioSystem::StaticTerminate();
    PhysicsWorld::StaticTerminate();
    ModelManager::StaticTerminate();
    TextureManager::StaticTerminate();
    SimpleDraw::StaticTerminate();
    DebugUI::StaticTerminate();
    InputSystem::StaticTerminate();
    GraphicsSystem::StaticTerminate();

    myWindow.Terminate();
}

void Application::ChangeState(const std::string& stateName)
{
    auto iter = mApplicationStates.find(stateName);

    if (iter != mApplicationStates.end())
        mNextState = iter->second.get();
}

bool Application::IsStateActive(const std::string& stateName)
{
    auto iter = mApplicationStates.find(stateName);
    return (iter != mApplicationStates.end()) && mCurrentState == iter->second.get();
}

void Application::Quit()
{
    mIsRunning = false;
}