#pragma once
#include "CustomTypeID.h"
#include <CultyEngine/Inc/GameObjectHandle.h>

using NoteDestroyedCallback = std::function<void(CultyEngine::GameObjectHandle, CultyEngine::MathC::Vector2)>;

class ComponentNote : public CultyEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentID::NoteMovement);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Deserialize(const rapidjson::Value& value) override;

    enum class MovementType { Line, Sine, Bezier };

    bool IsActive() const;
    void SetActive(bool active);
    float TimeRemaining() const;
    float GetTotalTime() const;
    bool HasDisappeared() const;

    bool IsCorrectButtonDown(int button) const;
    void SetRequiredButton(int button);

    CultyEngine::MathC::Vector2 GetCurrentPosition() const { return mPositionCurrent; }

    void SetOnDestroyedCallback(NoteDestroyedCallback cb);

private:
    NoteDestroyedCallback mOnDestroyedCallback = nullptr; // Callback for note destruction
    void SelfTerminate();

private:
    MovementType mMovementType = MovementType::Line;

    bool mIsActive = false;                         // Indicates if the note is in the active state
    float mTotalTravelTime = 0.0f;                  // Total time for note movement
    int mRequiredButton = -1;                       // Button required for this note

    float mSpeed = 0.0f;
    float mElapsedTime = 0.0f;
    float mFrequency = 1.0f;                        // Frequency for sine wave
    float mAmplitude = 35.0f;                       // Amplitude for sine wave
    CultyEngine::MathC::Vector2 mControlPoint;      // Control point for Bezier curve

    CultyEngine::MathC::Vector2 mPositionStart;
    CultyEngine::MathC::Vector2 mPositionEnd;
    CultyEngine::MathC::Vector2 mPositionCurrent;

    std::string mSilhouetteTemplatePath;             // Path to the silhouette template
    CultyEngine::GameObjectHandle mSilhouetteHandle; // Handle to the silhouette

    std::string mTimeArrowTemplatePath;              // Path to the arrow template
    CultyEngine::GameObjectHandle mTimeArrowHandle;  // Handle to the arrow sprite
};