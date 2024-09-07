#pragma once
#include "Animation.h"

namespace CultyEngine::Graphics
{
    class AnimationBuilder
    {
    public:
        AnimationBuilder& AddPositionKey(const MathC::Vector3& pos, float time, EaseType easeType = EaseType::Linear);
        AnimationBuilder& AddRotationKey(const MathC::Quaternion& rot, float time, EaseType easeType = EaseType::Linear);
        AnimationBuilder& AddScaleKey(const MathC::Vector3& scale, float time, EaseType easeType = EaseType::Linear);
        AnimationBuilder& AddEventKey(AnimationCallback cb, float time);

        [[nodiscard]] Animation Build();

    private:
        Animation mWorkingCopy;
    };
}