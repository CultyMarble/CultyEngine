#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

enum class CustomComponentID
{
    NoteMovement = static_cast<uint32_t>(CultyEngine::ComponentID::Count),
    FeedbackPopup = static_cast<uint32_t>(CultyEngine::ComponentID::Count) + 1
};