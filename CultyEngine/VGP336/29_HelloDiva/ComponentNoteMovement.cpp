#include "ComponentNoteMovement.h"
#include <CultyEngine/Inc/ComponentUISprite.h>
#include <CultyEngine/Inc/GameObject.h>
#include <CultyEngine/Inc/GameWorld.h>

using namespace CultyEngine;
using namespace CultyEngine::MathC;

void ComponentNoteMovement::Initialize()
{
    auto* sprite = GetOwner().GetComponent<ComponentUISprite>();
    if (sprite)
    {
        mPositionCurrent = mPositionStart; // Start at PositionStart
        sprite->SetPosition(mPositionCurrent);
    }
}

void ComponentNoteMovement::Terminate()
{

}

void ComponentNoteMovement::Update(float deltaTime)
{
    // Calculate the direction vector from Start to End
    Vector2 direction = mPositionEnd - mPositionStart;

    // Normalize the direction vector to maintain consistent speed in all directions
    if (direction.LengthSquared() > 0.0f) // Avoid division by zero
        direction = Normalize(direction);

    // Move the position in the direction of the normalized vector
    mPositionCurrent += direction * mSpeed * deltaTime;

    // Clamp the position when it reaches PositionEnd
    if ((direction.x > 0 && mPositionCurrent.x >= mPositionEnd.x) ||
        (direction.x < 0 && mPositionCurrent.x <= mPositionEnd.x) ||
        (direction.y > 0 && mPositionCurrent.y >= mPositionEnd.y) ||
        (direction.y < 0 && mPositionCurrent.y <= mPositionEnd.y))
    {
        mPositionCurrent = mPositionEnd;
    }

    // Update the sprite position
    auto* sprite = GetOwner().GetComponent<ComponentUISprite>();
    if (sprite)
        sprite->SetPosition(mPositionCurrent);

    // Delete the note when it reaches PositionEnd
    if (mPositionCurrent == mPositionEnd)
    {
        LOG("Note reached PositionEnd.");
        GetOwner().GetWorld().DestroyGameObject(GetOwner().GetHandle());
    }
}

void ComponentNoteMovement::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Speed"))
        mSpeed = value["Speed"].GetFloat();

    if (value.HasMember("PositionStart"))
    {
        auto pos = value["PositionStart"].GetArray();
        mPositionStart.x = pos[0].GetFloat();
        mPositionStart.y = pos[1].GetFloat();
    }

    if (value.HasMember("PositionEnd"))
    {
        auto pos = value["PositionEnd"].GetArray();
        mPositionEnd.x = pos[0].GetFloat();
        mPositionEnd.y = pos[1].GetFloat();
    }
}