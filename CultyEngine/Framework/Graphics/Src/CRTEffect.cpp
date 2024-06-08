#include "Precompiled.h"
#include "CRTEffect.h"
#include "RenderObject.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void CRTEffect::Initialize(const std::filesystem::path& filePath)
{
    mPostProcessingBuffer.Initialize();
    mVertexShader.Initialize<VertexPX>(filePath);
    mPixelShader.Initialize(filePath);
    mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void CRTEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mPostProcessingBuffer.Terminate();
}

void CRTEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    for (uint32_t i = 0; i < mTextures.size(); ++i)
    {
        if (mTextures[i] != nullptr)
            mTextures[i]->BindPS(i);
    }

    PostProcessData data;
    data.params0 = mAberrationValue;
    data.params1 = mScanLine;
    data.params2 = mUVOffsetY;

    mPostProcessingBuffer.Update(data);
    mPostProcessingBuffer.BindPS(0);
}

void CRTEffect::End()
{
    for (uint32_t i = 0; i < mTextures.size(); ++i)
        Texture::UnbindPS(i);
}

void CRTEffect::Update(float deltaTime)
{
    float randVal = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    mUVOffsetY -= deltaTime * (0.01f + (randVal * 0.03f));
}

void CRTEffect::Render(const RenderObject& renderObject)
{
    renderObject.meshBuffer.Render();
}

void CRTEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("CRT_Effect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("AberrationValue", &mAberrationValue, 0.001f, 0.0f, 1.0f);
        ImGui::DragInt("ScanLine", &mScanLine, 1, 0, 1000);
    }
}

void CRTEffect::SetTexture(const Texture* texture, uint32_t slot)
{
    ASSERT(slot < mTextures.size(), "PostProcessingEffect: Invalid slot index!");
    mTextures[slot] = texture;
}