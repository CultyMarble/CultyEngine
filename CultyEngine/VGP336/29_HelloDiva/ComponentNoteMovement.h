#pragma once
#include "CustomTypeID.h"
#include <CultyEngine/Inc/GameObjectHandle.h>

class ComponentNoteMovement : public CultyEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentID::NoteMovement);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Deserialize(const rapidjson::Value& value) override;

private:
    float mSpeed = 0.0f;
    float mElapsedTime = 0.0f;

    CultyEngine::MathC::Vector2 mPositionStart;
    CultyEngine::MathC::Vector2 mPositionEnd;
    CultyEngine::MathC::Vector2 mPositionCurrent;

    std::string mSilhouetteTemplatePath;             // Path to the silhouette template
    CultyEngine::GameObjectHandle mSilhouetteHandle; // Handle to the silhouette

    std::string mArrowTemplatePath;                  // Path to the arrow template
    CultyEngine::GameObjectHandle mArrowHandle;      // Handle to the arrow sprite
};