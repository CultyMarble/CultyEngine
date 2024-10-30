#pragma once
#include "Component.h"

namespace CultyEngine
{
    class ComponentCamera final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::Camera);

        void Initialize() override;
        void Terminate() override;
        void DebugUI() override;

        virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
        virtual void Deserialize(const rapidjson::Value& value) override;

        Graphics::Camera& GetCamera();
        const Graphics::Camera& GetCamera() const;

    private:
        Graphics::Camera mCamera;
    };
}