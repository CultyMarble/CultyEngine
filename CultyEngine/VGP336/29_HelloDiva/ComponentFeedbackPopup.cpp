#include "ComponentFeedbackPopup.h"

void ComponentFeedbackPopup::Initialize()
{
    // Ensure the GameObject has a UISprite component
    auto* sprite = GetOwner().GetComponent<CultyEngine::ComponentUISprite>();
    ASSERT(sprite != nullptr, "ComponentFeedbackPopup: UISprite component missing!");
}

void ComponentFeedbackPopup::Terminate()
{
    // Destroy the GameObject when the popup terminates
    GetOwner().GetWorld().DestroyGameObject(GetOwner().GetHandle());
}

void ComponentFeedbackPopup::Update(float deltaTime)
{
    mElapsedTime += deltaTime;
    if (mElapsedTime >= mLifetime)
        Terminate();
}

void ComponentFeedbackPopup::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Lifetime"))
        mLifetime = value["Lifetime"].GetFloat();
}

void ComponentFeedbackPopup::SetLifetime(float lifetime)
{
    mLifetime = lifetime;
}