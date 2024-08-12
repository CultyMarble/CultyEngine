#include "Precompiled.h"
#include "AnimationUtils.h"

#include "Model.h"
#include "Colors.h"
#include "SimpleDraw.h"
#include "Animator.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
    using namespace CultyEngine::Graphics::AnimationUtils;

    void ComputeBoneTransformsRecursive(const Bone* bone, BoneTransforms& boneTransforms, const Animator* animator)
    {
        if (bone != nullptr)
        {
            if (animator != nullptr)
            {
                boneTransforms[bone->index] = animator->GetToParentTransform(bone);
            }
            else
            {
                boneTransforms[bone->index] = bone->toParentTransform;
            }

            if (bone->parent != nullptr)
            {
                boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
            }

            for (const Bone* child : bone->children)
            {
                ComputeBoneTransformsRecursive(child, boneTransforms, animator);
            }
        }
    }
}

void CultyEngine::Graphics::AnimationUtils::ComputeBoneTransforms( ModelID id, BoneTransforms& boneTransforms, const Animator* animator)
{
    const Model* model = ModelManager::Get()->GetModel(id);
    if (model->skeleton != nullptr)
    {
        boneTransforms.resize(model->skeleton->bones.size(), MathC::Matrix4::Identity);
        ComputeBoneTransformsRecursive(model->skeleton->root, boneTransforms, animator);
    }
}

void AnimationUtils::ApplyBoneOffsets(ModelID id, BoneTransforms& boneTransforms)
{
    const Model* model = ModelManager::Get()->GetModel(id);
    if (model->skeleton != nullptr)
    {
        for (auto& bone : model->skeleton->bones)
        {
            boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
        }
    }
}

void AnimationUtils::DrawSkeleton(ModelID id, const BoneTransforms& boneTransforms)
{
    const Model* model = ModelManager::Get()->GetModel(id);
    if (model->skeleton != nullptr)
    {
        for (auto& bone : model->skeleton->bones)
        {
            if (bone->parent != nullptr)
            {
                const Vector3 posA = MathC::GetTranslation(boneTransforms[bone->index]);
                const Vector3 posB = MathC::GetTranslation(boneTransforms[bone->parentIndex]);

                if (MathC::DistanceSqr(posA, posB) > 0.0001f)
                {
                    SimpleDraw::AddLine(posA, posB, Colors::Teal);
                    SimpleDraw::AddSphere(10, 10, 0.03f, posA, Colors::Pink);
                }
            }
        }
    }
}