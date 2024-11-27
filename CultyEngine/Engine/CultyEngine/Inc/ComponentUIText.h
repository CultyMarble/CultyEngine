#pragma once
#include "ComponentUI.h"

namespace CultyEngine
{
    class ComponentUIText : public ComponentUI
    {
    public:
        SET_TYPE_ID(ComponentID::UIText);

        void Initialize() override;
        void Terminate() override;
        void Render() override;

        void Deserialize(const rapidjson::Value& value) override;

    private:
        std::filesystem::path mText;
        MathC::Vector2 mPosition = MathC::Vector2::Zero;
        float mSize = 1.0f;
        Color mColor = Colors::Black;
    };
}