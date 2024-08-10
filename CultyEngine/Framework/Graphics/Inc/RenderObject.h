#pragma once
#include "MeshBuffer.h"
#include "Material.h"
#include "TextureManager.h"
#include "Transform.h"
#include "ModelManager.h"

namespace CultyEngine::Graphics
{
    struct Model;

    struct RenderObject
    {
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;

        Material material;

        TextureID diffuseMapID;
        TextureID normalMapID;
        TextureID specularMapID;
        TextureID bumpMapID;
    };

    using RenderGroup = std::vector<RenderObject>;

    [[nodiscard]] RenderGroup CreateRenderGroup(ModelID id);
    [[nodiscard]] RenderGroup CreateRenderGroup(const Model& model);

    void CleanupRenderGroup(RenderGroup& renderGroup);

    void SetRenderGroupPosition(RenderGroup& renderGroup, const MathC::Vector3& newPosition);

    template<class Effect>
    void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
    {
        for (const RenderObject& renderObject : renderGroup)
        {
            effect.Render(renderObject);
        }
    }
}