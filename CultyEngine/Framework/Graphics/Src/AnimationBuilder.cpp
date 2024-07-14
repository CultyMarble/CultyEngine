#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
	template<class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float t, EaseType e)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= t, "AnimationBuilder: Cannot Add Keyframe Back in Time");
		keyframes.emplace_back(value, t, e);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const MathC::Vector3& pos, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mPositionKeys, pos, time, easeType);
	mWorkingCopy.mDuration = MathC::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const MathC::Quaternion& rot, float time, EaseType easeType)
{

}

AnimationBuilder& AnimationBuilder::AddScaleKey(const MathC::Vector3& scale, float time, EaseType easeType)
{

}
