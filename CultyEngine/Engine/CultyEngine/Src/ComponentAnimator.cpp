#include "Precompiled.h"
#include "ComponentAnimator.h"

#include "GameObject.h"
#include "ComponentModel.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void ComponentAnimator::Initialize()
{
    ComponentModel* modelComponent = GetOwner().GetComponent<ComponentModel>();
    mAnimator.Initialize(modelComponent->GetModelID());
}

void ComponentAnimator::Terminate()
{
    // nothing
}

void ComponentAnimator::Update(float deltaTime)
{
    mAnimator.Update(deltaTime);
}

void ComponentAnimator::DebugUI()
{
    std::string buttonTag = "PlayAnim";
    uint32_t animationCount = mAnimator.GetAnimationCount();
    for (uint32_t i = 0; i < animationCount; ++i)
    {
        std::string buttonName = buttonTag + std::to_string(i);
        if (ImGui::Button(buttonName.c_str()))
            Play(i, true);
    }
}

void ComponentAnimator::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    value.AddMember("ComponentAnimator", componentValue, doc.GetAllocator());
}

bool ComponentAnimator::Play(int index, bool looping)
{
    mAnimator.PlayAnimation(index, looping);
    return true;
}