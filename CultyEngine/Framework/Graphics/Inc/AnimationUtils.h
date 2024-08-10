#pragma once
#include "ModelManager.h"

namespace CultyEngine::Graphics
{
    namespace AnimationUtils
    {
        using BoneTransforms = std::vector<MathC::Matrix4>;

        void ComputeBoneTransforms(ModelID id, BoneTransforms& boneTransforms);
        void ApplyBoneOffsets(ModelID id, BoneTransforms& boneTransforms);
        void DrawSkeleton(ModelID id, const BoneTransforms& boneTransforms);
    }
}