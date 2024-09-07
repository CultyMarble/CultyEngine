#include "Precompiled.h"
#include "Animation.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
    float GetLerpTime(float start, float end, float time, EaseType easeType)
    {
        float t = (time - start) / (end - start);
        switch (easeType)
        {
        case EaseType::Linear:
            break;
        case EaseType::EaseInQuad:
            t = t * t;
            break;
        case EaseType::EaseOutQuad:
            t = t * (2.0f - t);
            break;
        case EaseType::EaseInOutQuad:
        {
            t *= 2.0f;
            if (t < 1.0f)
                t = 0.5f * t * t;
            else
            {
                t -= 1.0f;
                t = -0.5f * ((t * (t - 2.0f))) - 1.0f;
            }
        }
        break;
        default:
            break;
        }
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
    return mDuration;
}

void Animation::PlayEvents(float prevTime, float curTime)
{
    for (uint32_t i = 0; i < mEventKeys.size(); ++i)
    {
        if (mEventKeys[i].time > prevTime && mEventKeys[i].time <= curTime)
        {
            mEventKeys[i].key();
        }
    }
}

const MathC::Vector3& Animation::GetPosition(float time) const
{
    for (uint32_t i = 0; i < mPositionKeys.size(); ++i)
    {
        if (time < mPositionKeys[i].time)
        {
            if (i <= 0)
                return mPositionKeys[i].key;

            float t = GetLerpTime(
                mPositionKeys[i - 1].time,
                mPositionKeys[i].time,
                time,
                mPositionKeys[i].easeType);

            return MathC::Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, t);
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
            if (i <= 0)
                return mRotationKeys[i].key;

            float t = GetLerpTime(
                mRotationKeys[i - 1].time,
                mRotationKeys[i].time,
                time,
                mRotationKeys[i].easeType);

            return MathC::Quaternion::Slerp(mRotationKeys[i - 1].key, mRotationKeys[i].key, t);
        }
    }

    if (mRotationKeys.empty() == false)
        return mRotationKeys.back().key;

    return MathC::Quaternion::Identity;
}

const MathC::Vector3& Animation::GetScale(float time) const
{
    for (uint32_t i = 0; i < mScaleKeys.size(); ++i)
    {
        if (time < mScaleKeys[i].time)
        {
            if (i <= 0)
                return mScaleKeys[i].key;

            float t = GetLerpTime(
                mScaleKeys[i - 1].time,
                mScaleKeys[i].time,
                time,
                mScaleKeys[i].easeType);

            return MathC::Lerp(mScaleKeys[i - 1].key, mScaleKeys[i].key, t);
        }
    }

    if (mScaleKeys.empty() == false)
        return mScaleKeys.back().key;

    return MathC::Vector3::One;
}