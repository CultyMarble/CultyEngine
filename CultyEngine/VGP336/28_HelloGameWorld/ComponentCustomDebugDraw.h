#pragma once
#include "CustomTypeID.h"

class ComponentCustomDebugDraw : public CultyEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentID::CustomDebugDraw);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Deserialize(const rapidjson::Value& value) override;

    void AddDebugDraw();

private:
    const CultyEngine::ComponentTransform* mComponentTransform = nullptr;
    CultyEngine::MathC::Vector3 mPosition = CultyEngine::MathC::Vector3::Zero;
    CultyEngine::Color mColor = CultyEngine::Colors::White;
    uint32_t mSlices = 0;
    uint32_t mRings = 0;
    float mRadius = 0.0f;
};