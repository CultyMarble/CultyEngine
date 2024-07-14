#include "Precompiled.h"
#include "Animation.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
	float GetLerpTime(float start, float end, float time, EaseType easeType)
	{
		float t = (time - start) / (end - start);
		return t;
	}
}

Transform Animation::GetTransform(float time) const
{
	Transform transform;

	transform.position = GetPosition(time);
	transform.rotation = GetRotation(time);
	transform.scale = GetScale(time);

	return transform;
}

float Animation::GetDuration() const
{
	return 0.0f;
}

const MathC::Vector3& Animation::GetPosition(float time) const
{
	for (uint32_t i = 0; i < mPositionKeys.size(); ++i)
	{
		if (time < mPositionKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(
					mPositionKeys[i - 1].time,
					mPositionKeys[i].time,
					time,
					mPositionKeys[i].easeType);

				return MathC::Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, t);
			}
			return mPositionKeys[i].key;
		}
	}

	if (mPositionKeys.empty() == false)
		return mPositionKeys.back().key;

	return MathC::Vector3::Zero;
}

const MathC::Quaternion& Animation::GetRotation(float time) const
{
	for (uint32_t i = 0; i < mRotationKeys.size(); ++i)
	{
		if (time < mRotationKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(
					mRotationKeys[i - 1].time,
					mRotationKeys[i].time,
					time,
					mRotationKeys[i].easeType);

				return MathC::Slerp(mRotationKeys[i - 1].key, mPositionKeys[i].key, t);
			}
			return mRotationKeys[i].key;
		}
	}

	if (mPositionKeys.empty() == false)
		return mRotationKeys.back().key;

	return MathC::Quaternion::Identity;
}

const MathC::Vector3& Animation::GetScale(float time) const
{
	for (uint32_t i = 0; i < mScaleKeys.size(); ++i)
	{
		if (time < mScaleKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(
					mScaleKeys[i - 1].time,
					mScaleKeys[i].time,
					time,
					mScaleKeys[i].easeType);

				return MathC::Lerp(mScaleKeys[i - 1].key, mScaleKeys[i].key, t);
			}
			return mScaleKeys[i].key;
		}
	}

	if (mScaleKeys.empty() == false)
		return mScaleKeys.back().key;

	return MathC::Vector3::One;
}
