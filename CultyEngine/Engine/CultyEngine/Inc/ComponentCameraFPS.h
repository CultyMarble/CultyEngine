#pragma once
#include "Component.h"

namespace CultyEngine
{
    class ComponentCamera;

    class ComponentCameraFPS final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::CameraFPS);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;

        virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
        virtual void Deserialize(const rapidjson::Value& value) override;

    private:
        ComponentCamera* mCameraComponent = nullptr;
        float mShiftSpeed = 10.0f;
        float mMoveSpeed = 1.0f;
        float mTurnSpeed = 0.1f;
    };
}