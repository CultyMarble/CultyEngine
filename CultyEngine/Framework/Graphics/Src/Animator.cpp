#include "Precompiled.h"
#include "Animator.h"
#include "Animation.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void Animator::Initialize(ModelID id)
{
    mModelID = id;
    mIsLooping = false;
    mAnimationTick = 0.0f;
    mClipIndex = -1;
}

void Animator::PlayAnimation(int clipIndex, bool looping)
{
    if (clipIndex < static_cast<int>(GetAnimationCount()))
    {
        mClipIndex = clipIndex;
        mIsLooping = looping;
        mAnimationTick = 0.0f;
    }
}

void Animator::Update(float deltaTime)
{
    if (mClipIndex < 0)
        return;

    const Model* model = ModelManager::Get()->GetModel(mModelID);
    const AnimationClip& animClip = model->animationClips[mClipIndex];
    mAnimationTick += animClip.ticksPerSecond * deltaTime;

    if (mIsLooping)
    {
        while (mAnimationTick > animClip.tickDuration)
            mAnimationTick -= animClip.tickDuration;
    }
    else
    {
        mAnimationTick = std::min(mAnimationTick, animClip.tickDuration);
    }
}

bool Animator::IsFinished() const
{
    if (mIsLooping || mClipIndex < 0)
        return false;

    const Model* model = ModelManager::Get()->GetModel(mModelID);
    const AnimationClip& animClip = model->animationClips[mClipIndex];

    return mAnimationTick >= animClip.tickDuration;
}

size_t Animator::GetAnimationCount() const
{
    const Model* model = ModelManager::Get()->GetModel(mModelID);

    return model->animationClips.size();
}

MathC::Matrix4 CultyEngine::Graphics::Animator::GetToParentTransform(const Bone* bone) const
{
    if (mClipIndex < 0)
        return bone->toParentTransform;

    const Model* model = ModelManager::Get()->GetModel(mModelID);
    const AnimationClip& animClip = model->animationClips[mClipIndex];
    const Animation* animation = animClip.boneAnimations[bone->index].get();

    if (animation == nullptr)
        //return bone->toParentTransform;
        return MathC::Matrix4::Identity;

    Transform transform = animation->GetTransform(mAnimationTick);

    return transform.GetMatrix4();
}