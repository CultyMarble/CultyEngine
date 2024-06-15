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
    data.params0 = mAberrationSeperation;
    data.params1 = mScanlineFrequency;
    data.params2 = mScanlineAlpha;
    data.params3 = mScanlineBrightness;
    data.params4 = mScanlineOffsetY;
    data.params5 = mBloomThreshold;
    data.params6 = mBloomStrength;
    data.params7 = mNoiseBrightness;
    data.params8 = mNoiseAlpha;
    data.params9 = mNoiseRandom;
    data.params10 = mJitterThickness;
    data.params11 = mJitterStrength;
    data.params12 = mJitterRandom;

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
    mScanlineOffsetY -= deltaTime * (0.01f + (randVal * 0.03f));
    if (mScanlineOffsetY <= -1.0)
        mScanlineOffsetY = 0.0f;

    mNoiseRandom += deltaTime * 5.0f;
    if (mScanlineOffsetY > 100.0f)
        mScanlineOffsetY = 0.0f;

    mJitterRandom += deltaTime * (1.0f + (randVal * 5.0f));
    if (mJitterThickness > 100.0f)
        mJitterThickness = 0.0f;
}

void CRTEffect::Render(const RenderObject& renderObject)
{
    renderObject.meshBuffer.Render();
}

void CRTEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("CRT Effect Config", ImGuiTreeNodeFlags_CollapsingHeader))
    {
        ImGui::DragFloat("Aberration Seperation", &mAberrationSeperation, 0.001f, 0.0f, 1.0f);

        ImGui::DragInt  ("Scanline Frequency",    &mScanlineFrequency,    1, 0, 1000);
        ImGui::DragFloat("Scanline Brightness",   &mScanlineBrightness,   0.001f, 0.0f, 2.0f);
        ImGui::DragFloat("Scanline Alpha",        &mScanlineAlpha,        0.001f, 0.0f, 1.0f);

        ImGui::DragFloat("Bloom Threshold", &mBloomThreshold,       0.001f, 0.0f, 1.0f);
        ImGui::DragFloat("Bloom Strength",  &mBloomStrength,        0.01f, 0.0f, 10.0f);

        ImGui::DragFloat("Noise Brightness", &mNoiseBrightness, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat("Noise Alpha",      &mNoiseAlpha, 0.01f, 0.0f, 1.0f);

        ImGui::DragFloat("Jitter Thickness",    &mJitterThickness, 0.5f, 0.0f, 5.0f);
        ImGui::DragFloat("Jitter Strength", &mJitterStrength, 0.001f, 0.0f, 1.0f);
    }
}

void CRTEffect::SetTexture(const Texture* texture, uint32_t slot)
{
    ASSERT(slot < mTextures.size(), "PostProcessingEffect: Invalid slot index!");
    mTextures[slot] = texture;
}