#include "Precompiled.h"
#include "PostProcessingEffect.h"
#include "RenderObject.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
    const char* gModeName[] =
    {
        "None",
        "Monochrome",
        "Invert",
        "Mirror",
        "Blur",
        "Combine2",
        "ChromaticAberration",
        "Wave",
    };
}

void PostProcessingEffect::Initialize(const std::filesystem::path& filePath)
{
    mPostProcessingBuffer.Initialize();
    mVertexShader.Initialize<VertexPX>(filePath);
    mPixelShader.Initialize(filePath);

    mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void PostProcessingEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mPostProcessingBuffer.Terminate();
}

void PostProcessingEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    for (uint32_t i = 0; i < mTextures.size(); ++i)
    {
        if (mTextures[i] != nullptr)
            mTextures[i]->BindPS(i);
    }

    PostProcessData ppData;
    ppData.mode = static_cast<int>(mMode);
    switch (mMode)
    {
    case Mode::None: break;
    case Mode::Monochrome: break;
    case Mode::Invert: break;
    case Mode::Mirror:
    {
        ppData.params0 = mMirrorX;
        ppData.params1 = mMirrorY;
    }
    break;
    case Mode::Blur:
    {
        GraphicsSystem* gs = GraphicsSystem::Get();
        const float screenWidth = gs->GetBackBufferWidth();
        const float screenHeigth = gs->GetBackBufferHeight();
        ppData.params0 = mBlurStrength / screenWidth;
        ppData.params1 = mBlurStrength / screenHeigth;
    }
    break;
    case Mode::Combine2:
    {
        // Extra
        // ppData.params0 = mUVOffsetX;
    }
    break;
    case Mode::ChromaticAberration:
    {
        ppData.params0 = mAberrationValue;
        ppData.params1 = mAberrationValue;
    }
    break;
    case Mode::Wave:
    {
        ppData.params0 = mWaveCount;
        ppData.params1 = mWaveLength;
    }
    break;
    default:
        break;
    }

    mPostProcessingBuffer.Update(ppData);
    mPostProcessingBuffer.BindPS(0);
}

void PostProcessingEffect::End()
{
    for (uint32_t i = 0; i < mTextures.size(); ++i)
    {
        Texture::UnbindPS(i);
    }
}

//void PostProcessingEffect::Update(float deltaTime)
//{
//    mUVOffsetX += deltaTime;
//}

void PostProcessingEffect::Render(const RenderObject& renderObject)
{
    renderObject.meshBuffer.Render();
}

void PostProcessingEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("PostProcessingEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        int currentMode = static_cast<int>(mMode);

        if (ImGui::Combo("Mode", &currentMode, gModeName, std::size(gModeName)))
            mMode = static_cast<Mode>(currentMode);

        ImGui::DragFloat("MirrorX", &mMirrorX, 0.1f, -1.0f, 1.0f);
        ImGui::DragFloat("MirrorY", &mMirrorY, 0.1f, -1.0f, 1.0f);
        ImGui::DragFloat("BlurStrength", &mBlurStrength, 1.0f, 0.0f, 100.0f);
        ImGui::DragFloat("AberrationValue", &mAberrationValue, 0.001f, 0.0f, 1.0f);
        ImGui::DragInt("Wave Count", &mWaveCount, 1, 0, 1000);
        ImGui::DragFloat("Wave Length", &mWaveLength, 0.001f, 0.0f, 1.0f);

        // Extra
        // ImGui::DragFloat("UVOffsetX", &mUVOffsetX, 0.001f);
    }
}

void PostProcessingEffect::SetMode(Mode mode)
{
    mMode = mode;
}

void PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
    ASSERT(slot < mTextures.size(), "PostProcessingEffect: Invalid slot index!");
    mTextures[slot] = texture;
}