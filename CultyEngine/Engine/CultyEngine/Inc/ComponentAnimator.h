#pragma once
#include "Component.h"

namespace CultyEngine
{
    class ComponentAnimator final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::Animator);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;
        void DebugUI() override;

        void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;

        bool Play(int index, bool looping = false);

        Graphics::Animator& GetAnimator() { return mAnimator; };
        const Graphics::Animator& GetAnimator() const { return mAnimator; };

    private:
        Graphics::Animator mAnimator;
    };
}