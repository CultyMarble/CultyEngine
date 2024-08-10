#include "Precompiled.h"
#include "AnimationUtils.h"

#include "Model.h"
#include "Colors.h"
#include "SimpleDraw.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
    using namespace CultyEngine::Graphics::AnimationUtils;

    void ComputeBoneTransformsRecursive(const Bone* bone, BoneTransforms& boneTransforms)
    {
        if (bone != nullptr)
        {
            boneTransforms[bone->index] = bone->toParentTransform;
            if (bone->parent != nullptr)
            {
                boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
            }

            for (const Bone* child : bone->children)
            {
                ComputeBoneTransformsRecursive(child, boneTransforms);
            }
        }
    }
}

void AnimationUtils::ComputeBoneTransforms(ModelID id, BoneTransforms& boneTransforms)
{
    const Model* model = ModelManager::Get()->GetModel(id);
    if (model->skeleton != nullptr)
    {
        boneTransforms.resize(model->skeleton->bones.size(), MathC::Matrix4::Identity);
        ComputeBoneTransformsRecursive(model->skeleton->root, boneTransforms);
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
                    SimpleDraw::AddLine(posA, posB, Colors::Blue);
                    SimpleDraw::AddSphere(10, 10, 0.03f, posA, Colors::Pink);
                }
            }
        }
    }
}