#pragma once
#include "Component.h"

namespace CultyEngine
{
    class ComponentTransform final : public Component, public Graphics::Transform
    {
    public:
        SET_TYPE_ID(ComponentID::Transform);

        void DebugUI() override;
        virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
        virtual void Deserialize(const rapidjson::Value& value) override;
    };
}