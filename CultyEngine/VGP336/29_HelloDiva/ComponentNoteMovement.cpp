#include "ComponentNoteMovement.h"
#include <CultyEngine/Inc/ComponentUISprite.h>
#include <CultyEngine/Inc/GameObject.h>
#include <CultyEngine/Inc/GameWorld.h>

using namespace CultyEngine;
using namespace CultyEngine::MathC;

void ComponentNoteMovement::Initialize()
{
    // Spawn the silhouette at PositionEnd
    if (!mSilhouetteTemplatePath.empty())
    {
        GameObject* silhouetteObject = GetOwner().GetWorld().CreateGameObject("Silhouette", mSilhouetteTemplatePath);
        if (silhouetteObject)
        {
            // Place the silhouette at PositionEnd
            auto* sprite = silhouetteObject->GetComponent<ComponentUISprite>();
            if (sprite)
                sprite->SetPosition(mPositionEnd);

            silhouetteObject->Initialize();
            mSilhouetteHandle = silhouetteObject->GetHandle();
        }
    }

    // Place the note at PositionStart
    auto* sprite = GetOwner().GetComponent<ComponentUISprite>();
    if (sprite)
    {
        mPositionCurrent = mPositionStart;
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
        LOG("ComponentNoteMovement: Note reached PositionEnd and will be deleted.");
        GetOwner().GetWorld().DestroyGameObject(mSilhouetteHandle);
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

    if (value.HasMember("SilhouetteTemplate"))
    {
        mSilhouetteTemplatePath = value["SilhouetteTemplate"].GetString();
    }
}