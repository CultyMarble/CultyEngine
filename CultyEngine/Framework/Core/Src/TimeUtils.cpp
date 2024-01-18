#include "Precompiled.h"
#include "TimeUtils.h"

namespace CultyEngine::Core
{
    float TimeUtils::GetTime()
    {
        static auto startTime = std::chrono::high_resolution_clock::now();
        const auto currentTime = std::chrono::high_resolution_clock::now();
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        return milliseconds / 1000.0f;
    }

    float TimeUtils::GetDeltaTime()
    {
        static auto lastCallTime = std::chrono::high_resolution_clock::now();
        const auto currentTime = std::chrono::high_resolution_clock::now();
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCallTime).count();
        lastCallTime = currentTime;

        return milliseconds / 1000.0f;
    }
}