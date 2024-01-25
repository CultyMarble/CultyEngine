#pragma once

namespace CultyEngine
{
    class ApplicationState;

    struct ApplicationConfig
    {
        std::wstring applicationName = L"ApplicationName";

        uint32_t winWidth = 1280;   // default
        uint32_t winHeight = 720;   // default
    };

    class Application final
    {
    public:
        template<class StateType> 
        void AddState(const std::string& stateName)
        {
            static_assert(std::is_base_of_v<ApplicationState, StateType>, "AddState must be of type ApplicationState");

            auto [iter, success] = mApplicationStates.try_emplace(stateName, nullptr);
            if (success)
            {
                auto& ptr = iter->second;
                ptr = std::make_unique<StateType>();
                if (mCurrentState == nullptr)
                {
                    LOG("Application: Current state %s", stateName.c_str());
                    mCurrentState = ptr.get();
                }
            }
        }

        void ChangeState(const std::string& stateName);

        void Run(const ApplicationConfig& config);
        void Quit();

    private:
        using ApplicationStateMap = std::map<std::string, std::unique_ptr<ApplicationState>>;

        ApplicationStateMap mApplicationStates;
        ApplicationState* mCurrentState = nullptr;
        ApplicationState* mNextState = nullptr;

        bool mIsRunning = false;
    };
}