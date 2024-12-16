#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

enum class CustomComponentID
{
    CustomDebugDraw = static_cast<uint32_t>(CultyEngine::ComponentID::Count)
};

enum class CustomServiceID
{
    CustomDebugDrawDisplay = static_cast<uint32_t>(CultyEngine::ServiceID::Count)
};