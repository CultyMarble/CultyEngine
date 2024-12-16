#include "ComponentNoteMovement.h"

#include <CultyEngine/Inc/ComponentUISprite.h>

using namespace CultyEngine;
using namespace CultyEngine::MathC;

void ComponentNoteMovement::Initialize()
{
    auto* sprite = GetOwner().GetComponent<ComponentUISprite>();
    if (sprite)
        mPosition = sprite->GetPosition(false);
}

void ComponentNoteMovement::Terminate()
{

}

void ComponentNoteMovement::Update(float deltaTime)
{
    mPosition.x += mSpeed * deltaTime;
    auto* sprite = GetOwner().GetComponent<ComponentUISprite>();
    if (sprite)
        sprite->SetPosition(mPosition);
}

void ComponentNoteMovement::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Speed"))
        mSpeed = value["Speed"].GetFloat();

    if (value.HasMember("StartPosition"))
    {
        auto pos = value["StartPosition"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
    }
}