#include "Precompiled.h"
#include "Application.h"
#include "ApplicationState.h"

using namespace CultyEngine;
using namespace CultyEngine::Core;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

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

    auto windowHandle = myWindow.GetWindowHandle();

    Graphics::GraphicsSystem::StaticInitialize(windowHandle, false);
    Input::InputSystem::StaticInitialize(windowHandle);
    SimpleDraw::StaticInitialize(config.maxVertexCount);

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

        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }

        float deltaTime = TimeUtils::GetDeltaTime();
        mCurrentState->Update(deltaTime);

        // Render
        Graphics::GraphicsSystem* gs = Graphics::GraphicsSystem::Get();
        gs->BeginRender();
        mCurrentState->Render();
        gs->EndRender();
    }

    // Clean Up
    mCurrentState->Terminate();

    SimpleDraw::StaticTerminate();
    Input::InputSystem::StaticTerminate();
    Graphics::GraphicsSystem::StaticTerminate();

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