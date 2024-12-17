#pragma once
#include "ComponentUI.h"

namespace CultyEngine
{
    class ComponentUISprite : public ComponentUI
    {
    public:
        SET_TYPE_ID(ComponentID::UISprite);

        void Initialize() override;
        void Terminate() override;
        void Render() override;
        void Deserialize(const rapidjson::Value& value) override;

        MathC::Vector2 GetPosition(bool includeOrigin = true);
        void SetPosition(const MathC::Vector2& position) { mPosition = { position.x, position.y }; }

        float GetRotation() const { return mRotation; }
        void SetRotation(float rotation) { mRotation = rotation; mUISprite.SetRotation(rotation); }

    private:
        std::filesystem::path mTexturePath;
        DirectX::XMFLOAT2 mPosition;
        float mRotation = 0.0f;
        RECT mRect = { 0, 0, 0, 0 };
        Graphics::UISprite mUISprite;
    };
}