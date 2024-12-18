#pragma once
#include "CustomTypeID.h"
#include <CultyEngine/Inc/Component.h>
#include <CultyEngine/Inc/GameObjectHandle.h>

class ComponentFeedbackPopup final : public CultyEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentID::FeedbackPopup);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Deserialize(const rapidjson::Value& value) override;

    void SetLifetime(float lifetime);

private:
    float mLifetime = 1.0f; // Default lifetime in seconds
    float mElapsedTime = 0.0f;
};