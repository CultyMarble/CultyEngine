#include "ComponentNoteMovement.h"
#include <CultyEngine/Inc/ComponentUISprite.h>
#include <CultyEngine/Inc/GameObject.h>
#include <CultyEngine/Inc/GameWorld.h>
#include <CultyEngine/Inc/GameObjectFactory.h>

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

    // Spawn the arrow at PositionEnd
    if (!mArrowTemplatePath.empty())
    {
        GameObject* arrowObject = GetOwner().GetWorld().CreateGameObject("Arrow", mArrowTemplatePath);
        if (arrowObject)
        {
            auto* arrowSprite = arrowObject->GetComponent<ComponentUISprite>();
            if (arrowSprite)
                arrowSprite->SetPosition(mPositionEnd); // Place arrow at silhouette position

            arrowObject->Initialize();
            mArrowHandle = arrowObject->GetHandle();
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
    // Increment elapsed time for this note
    mElapsedTime += deltaTime;

    // Calculate total travel time
    float totalTravelTime = (mPositionEnd - mPositionStart).Length() / mSpeed;

    // Compute the rotation angle in degrees
    float rotationAngleDegrees = 360.0f * (mElapsedTime / totalTravelTime);
    if (rotationAngleDegrees > 360.0f)
        rotationAngleDegrees = 360.0f;

    // Convert degrees to radians
    float rotationAngleRadians = rotationAngleDegrees * (static_cast<float>(M_PI) / 180.0f);

    // Set arrow rotation
    GameObject* arrowObject = GetOwner().GetWorld().GetGameObject(mArrowHandle);
    if (arrowObject)
    {
        auto* arrowSprite = arrowObject->GetComponent<ComponentUISprite>();
        if (arrowSprite)
            arrowSprite->SetRotation(rotationAngleRadians);
    }

    // Move the note
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
        GetOwner().GetWorld().DestroyGameObject(mSilhouetteHandle);
        GetOwner().GetWorld().DestroyGameObject(mArrowHandle);
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

    if (value.HasMember("ArrowTemplate"))
    {
        mArrowTemplatePath = value["ArrowTemplate"].GetString();
    }
}