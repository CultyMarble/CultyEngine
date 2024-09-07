#pragma once
#include "Keyframe.h"
#include "Transform.h"

namespace CultyEngine::Graphics
{
	class Animation
	{
	public:
		Transform GetTransform(float time) const;
		float GetDuration() const;

        void PlayEvents(float prevTime, float curTime);

	private:
		const MathC::Vector3& GetPosition(float time) const;
		const MathC::Quaternion& GetRotation(float time) const;
		const MathC::Vector3& GetScale(float time) const;

        friend class AnimationBuilder;
        friend class AnimationIO;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
        EventKeys mEventKeys;
		float mDuration;
	};
}