#include "Precompiled.h"
#include "RenderObject.h"
#include "Model.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}

RenderGroup Graphics::CreateRenderGroup(const Model & model)
{
    auto TryLoadTexure = [](const auto& textureName)->TextureID
        {
            if (textureName.empty())
                return 0;

            return TextureManager::Get()->LoadTexture(textureName, false);
        };

    RenderGroup renderGroup;
    renderGroup.reserve(model.meshData.size());
    for (const Model::MeshData& meshData : model.meshData)
    {
        RenderObject& renderObject = renderGroup.emplace_back();
        renderObject.meshBuffer.Initialize(meshData.mesh);
        if (meshData.materialIndex < model.materialData.size())
        {
            const Model::MaterialData& materialData = model.materialData[meshData.materialIndex];
            renderObject.material = materialData.material;
            renderObject.diffuseMapID = TryLoadTexure(materialData.diffuseMapName);
            renderObject.normalMapID = TryLoadTexure(materialData.normalMapName);
            renderObject.bumpMapID = TryLoadTexure(materialData.bumpMapName);
            renderObject.specularMapID = TryLoadTexure(materialData.specularMapName);
        }
    }

    return renderGroup;
}

void Graphics::CleanupRenderGroup(RenderGroup& renderGroup)
{
    for (RenderObject& renderObject : renderGroup)
    {
        renderObject.Terminate();
    }
}

void Graphics::SetRenderGroupPosition(RenderGroup& renderGroup, const MathC::Vector3& newPosition)
{
    for (RenderObject& renderObject : renderGroup)
    {
        renderObject.transform.position = newPosition;
    }
}