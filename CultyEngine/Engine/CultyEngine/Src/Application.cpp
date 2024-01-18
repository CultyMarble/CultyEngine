#include "Precompiled.h"
#include "Application.h"
#include "ApplicationState.h"

namespace CultyEngine
{
    void Application::Run(const ApplicationConfig& config)
    {
        LOG("Application Started: %.3f", TimeUtils::GetTime());

        Window _mWindow;
        _mWindow.Initialize(
            GetModuleHandle(nullptr),
            config.applicationName,
            config.winWidth,
            config.winHeight
        );

        ASSERT(_mWindow.IsActive(), "Failed to create a Window!");
        ASSERT(mCurrentState != nullptr, "Application: need an application state!");
        mCurrentState->Initialize();

        mIsRunning = true;
        while (mIsRunning)
        {
            _mWindow.ProcessMessage();

            float deltaTime = TimeUtils::GetDeltaTime();

            if (_mWindow.IsActive() == false)
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
        }

        mCurrentState->Terminate();
        _mWindow.Terminate();
    }

    void Application::ChangeState(const std::string& stateName)
    {
        auto iter = mApplicationStates.find(stateName);
        if (iter != mApplicationStates.end())
        {
            mNextState = iter->second.get();
        }
    }

    void Application::Quit()
    {
        mIsRunning = false;
    }
}