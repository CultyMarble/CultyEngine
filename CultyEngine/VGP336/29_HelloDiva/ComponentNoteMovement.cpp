#include "ComponentNoteMovement.h"
#include <CultyEngine/Inc/ComponentUISprite.h>
#include <CultyEngine/Inc/GameObject.h>
#include <CultyEngine/Inc/GameWorld.h>
#include <CultyEngine/Inc/GameObjectFactory.h>

using namespace CultyEngine;

namespace
{
    void SpawnSilhouette(const std::string& silhouetteTemplatePath, GameObject& owner, const Vector2& positionEnd, GameObjectHandle& silhouetteHandle)
    {
        if (!silhouetteTemplatePath.empty())
        {
            GameObject* silhouetteObject = owner.GetWorld().CreateGameObject("Silhouette", silhouetteTemplatePath);
            if (silhouetteObject)
            {
                // Place the silhouette at PositionEnd
                auto* sprite = silhouetteObject->GetComponent<ComponentUISprite>();
                if (sprite)
                    sprite->SetPosition(positionEnd);

                silhouetteObject->Initialize();
                silhouetteHandle = silhouetteObject->GetHandle();
            }
        }
    }

    void SpawnTimeArrow(const std::string& mArrowTemplatePath, GameObject& owner, const Vector2& positionEnd, GameObjectHandle& arrowHandle)
    {
        if (!mArrowTemplatePath.empty())
        {
            GameObject* arrowObject = owner.GetWorld().CreateGameObject("Arrow", mArrowTemplatePath);
            if (arrowObject)
            {
                auto* arrowSprite = arrowObject->GetComponent<ComponentUISprite>();
                if (arrowSprite)
                    arrowSprite->SetPosition(positionEnd); // Place arrow at silhouette position

                arrowObject->Initialize();
                arrowHandle = arrowObject->GetHandle();
            }
        }
    }

    // Calculate rotation angle in radians based on elapsed and total travel time
    float CalculateRotationAngle(const float& elapsedTime, const float& totalTravelTime)
    {
        float rotationAngleDegrees = 360.0f * (elapsedTime / totalTravelTime);
        if (rotationAngleDegrees > 360.0f)
            rotationAngleDegrees = 360.0f;
        return rotationAngleDegrees * (static_cast<float>(M_PI) / 180.0f); // Convert to radians
    }

    void UpdateTimeArrow(const float& totalTravelTime, const float& elapsedTime, GameObject& owner, const GameObjectHandle& arrowHandle)
    {
        // Calculate arrow rotation
        float rotationAngleRadians = CalculateRotationAngle(elapsedTime, totalTravelTime);

        // Set arrow rotation
        GameObject* arrowObject = owner.GetWorld().GetGameObject(arrowHandle);
        if (arrowObject)
        {
            ComponentUISprite* arrowSprite = arrowObject->GetComponent<ComponentUISprite>();
            if (arrowSprite)
                arrowSprite->SetRotation(rotationAngleRadians);
        }
    }

    // Update position along a line
    Vector2 UpdateLineMovement(const Vector2& positionStart, const Vector2& positionEnd, const float& speed, const float& deltaTime, Vector2& positionCurrent)
    {
        Vector2 direction = positionEnd - positionStart;
        if (direction.LengthSquared() > 0.0f) // Avoid division by zero
            direction = Normalize(direction);

        positionCurrent += direction * speed * deltaTime;

        // Clamp the position when it reaches PositionEnd
        if ((direction.x > 0 && positionCurrent.x >= positionEnd.x) ||
            (direction.x < 0 && positionCurrent.x <= positionEnd.x) ||
            (direction.y > 0 && positionCurrent.y >= positionEnd.y) ||
            (direction.y < 0 && positionCurrent.y <= positionEnd.y))
        {
            positionCurrent = positionEnd;
        }

        return positionCurrent;
    }

    // Update position with sine wave movement
    Vector2 UpdateSineMovement(const Vector2& positionStart, const Vector2& positionEnd, const float& totalTravelTime, const float& amplitude, const float& frequency, const float& elapsedTime, Vector2& positionCurrent)
    {
        // Calculate the normalized direction from start to end
        Vector2 direction = positionEnd - positionStart;
        float distance = direction.Length();
        direction = Normalize(direction);

        // Calculate movement progress (0.0 to 1.0)
        float t = MathC::Clamp(elapsedTime / totalTravelTime, 0.0f, 1.0f);

        // Move along the straight line (forward progress)
        Vector2 forwardMovement = positionStart + direction * distance * t;

        // Add sine wave oscillation perpendicular to the movement direction
        Vector2 perpendicular = direction.Perpendicular();
        float sineOffset = amplitude * sinf(frequency * t * 2.0f * static_cast<float>(M_PI));

        // Combine forward movement with perpendicular oscillation
        positionCurrent = forwardMovement + perpendicular * sineOffset;

        return positionCurrent;
    }

    // Update position along a Bezier curve
    Vector2 UpdateBezierMovement(const Vector2& positionStart, const Vector2& positionEnd, const Vector2& controlPoint, const float& elapsedTime, const float& totalTravelTime, Vector2& positionCurrent)
    {
        float t = Clamp(elapsedTime / totalTravelTime, 0.0f, 1.0f); // Ensure t is between 0 and 1
        positionCurrent = (1 - t) * (1 - t) * positionStart + 2 * (1 - t) * t * controlPoint + t * t * positionEnd;
        return positionCurrent;
    }
}

void ComponentNoteMovement::Initialize()
{
    // Spawn note silhouette at PositionEnd
    SpawnSilhouette(mSilhouetteTemplatePath, GetOwner(), mPositionEnd, mSilhouetteHandle);

    // Spawn silhouette time arrow at PositionEnd
    SpawnTimeArrow(mTimeArrowTemplatePath, GetOwner(), mPositionEnd, mTimeArrowHandle);

    // Calculate total travel time for movement
    mTotalTravelTime = (mPositionStart - mPositionEnd).Length() / mSpeed;

    // Default note is inactive at the start
    mIsActive = false;

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
    GetOwner().GetWorld().DestroyGameObject(mSilhouetteHandle);
    GetOwner().GetWorld().DestroyGameObject(mTimeArrowHandle);
    GetOwner().GetWorld().DestroyGameObject(GetOwner().GetHandle());
}

void ComponentNoteMovement::Update(float deltaTime)
{
    // Increment elapsed time for this note
    mElapsedTime += deltaTime;

    // Calculate total travel time
    float totalTravelTime = (mPositionStart - mPositionEnd).Length() / mSpeed;

    // Update the rotation of time arrow
    UpdateTimeArrow(totalTravelTime, mElapsedTime, GetOwner(), mTimeArrowHandle);

    // Update position based on movement type
    switch (mMovementType)
    {
    case MovementType::Line:
        UpdateLineMovement(mPositionStart, mPositionEnd, mSpeed, deltaTime, mPositionCurrent);
        break;
    case MovementType::Sine:
        UpdateSineMovement(mPositionStart, mPositionEnd, totalTravelTime, mAmplitude, mFrequency, mElapsedTime, mPositionCurrent);
        break;
    case MovementType::Bezier:
        UpdateBezierMovement(mPositionStart, mPositionEnd, mControlPoint, mElapsedTime, totalTravelTime, mPositionCurrent);
        break;
    }

    // Update the sprite position
    ComponentUISprite* sprite = GetOwner().GetComponent<ComponentUISprite>();
    sprite->SetPosition(mPositionCurrent);

    // Delete the note when it reaches PositionEnd
    if ((mPositionCurrent - mPositionEnd).LengthSquared() <= MathC::EPSILON * MathC::EPSILON)
    {
        mPositionCurrent = mPositionEnd;
        SelfTerminate();
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
        mTimeArrowTemplatePath = value["ArrowTemplate"].GetString();
    }

    if (value.HasMember("MovementType"))
    {
        std::string movementTypeStr = value["MovementType"].GetString();
        if (movementTypeStr == "Sine")
        {
            mMovementType = MovementType::Sine;

            if (value.HasMember("Frequency"))
                mFrequency = value["Frequency"].GetFloat();

            if (value.HasMember("Amplitude"))
                mAmplitude = value["Amplitude"].GetFloat();
        }
        else if (movementTypeStr == "Bezier")
        {
            mMovementType = MovementType::Bezier;

            // Calculate the control point
            Vector2 direction = mPositionEnd - mPositionStart;
            float distance = direction.Length();

            // Position along the line
            Vector2 midPoint = mPositionStart + direction * 0.7f;

            // Calculate perpendicular offset
            Vector2 perpendicular = Normalize(direction).Perpendicular();
            float intensity = 0.3f * distance; // Adjust intensity
            mControlPoint = midPoint + perpendicular * intensity;
        }
        else
        {
            mMovementType = MovementType::Line; // Default
        }
    }

    if (value.HasMember("RequiredButton"))
    {
        mRequiredButton = value["RequiredButton"].GetInt();
    }
}

bool ComponentNoteMovement::IsActive() const
{
    return mIsActive;
}

void ComponentNoteMovement::SetActive(bool active)
{
    mIsActive = active;
}

float ComponentNoteMovement::TimeRemaining() const
{
    return mTotalTravelTime - mElapsedTime;
}

float ComponentNoteMovement::GetTotalTime() const
{
    return mTotalTravelTime;
}

bool ComponentNoteMovement::HasDisappeared() const
{
    return TimeRemaining() <= 0.0f;
}

bool ComponentNoteMovement::IsCorrectButton(int button) const
{
    return button == mRequiredButton;
}

void ComponentNoteMovement::SetRequiredButton(int button)
{
    mRequiredButton = button;
}

void ComponentNoteMovement::SetOnDestroyedCallback(NoteDestroyedCallback cb)
{
    mOnDestroyedCallback = cb;
}

void ComponentNoteMovement::SelfTerminate()
{
    if (mOnDestroyedCallback)
        mOnDestroyedCallback(GetOwner().GetHandle());

    GetOwner().GetWorld().DestroyGameObject(mSilhouetteHandle);
    GetOwner().GetWorld().DestroyGameObject(mTimeArrowHandle);
    GetOwner().GetWorld().DestroyGameObject(GetOwner().GetHandle());
}