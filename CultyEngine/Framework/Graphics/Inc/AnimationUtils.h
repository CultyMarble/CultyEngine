#pragma once
#include "ModelManager.h"

namespace CultyEngine::Graphics
{
    class Animator;

    namespace AnimationUtils
    {
        using BoneTransforms = std::vector<MathC::Matrix4>;

        void ComputeBoneTransforms(ModelID id, BoneTransforms& boneTransforms, const Animator* animator = nullptr);
        void ApplyBoneOffsets(ModelID id, BoneTransforms& boneTransforms);
        void DrawSkeleton(ModelID id, const BoneTransforms& boneTransforms);
    }
}