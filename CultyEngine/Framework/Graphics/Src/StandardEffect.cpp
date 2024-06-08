#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& filePath)
{
    mTransformBuffer.Initialize();
    mSettingsBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mVertexShader.Initialize<Vertex>(filePath);
    mPixelShader.Initialize(filePath);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mSettingsBuffer.Terminate();
    mTransformBuffer.Terminate();
}

void StandardEffect::Begin()
{
    ASSERT(mCamera != nullptr, "StandardEffect: No Camera Set!");
    ASSERT(mDirectionalLight != nullptr, "StandardEffect: No Light Set!");

    mVertexShader.Bind();
    mPixelShader.Bind();

    mSampler.BindVS(0);
    mSampler.BindPS(0);

    mTransformBuffer.BindVS(0);

    mSettingsBuffer.BindVS(1);
    mSettingsBuffer.BindPS(1);

    mLightBuffer.BindVS(2);
    mLightBuffer.BindPS(2);

    mMaterialBuffer.BindPS(3);
}

void StandardEffect::End()
{
    if (mShadowMap == nullptr)
        return;

    Texture::UnbindPS(4);
}

void StandardEffect::Render(const RenderObject& renderObject)
{
    SettingsData settingsData;
    settingsData.useDiffuseMap = renderObject.diffuseMapID > 0 && mSettingsData.useDiffuseMap > 0 ? 1 : 0;
    settingsData.useNormalMap = renderObject.normalMapID > 0 && mSettingsData.useNormalMap > 0 ? 1 : 0;
    settingsData.useSpecularMap = renderObject.specularMapID > 0 && mSettingsData.useSpecularMap > 0 ? 1 : 0;
    settingsData.useLighting = mSettingsData.useLighting > 0 ? 1 : 0;
    settingsData.useBumpMap = renderObject.bumpMapID > 0 && mSettingsData.useBumpMap > 0;
    settingsData.bumpWeight = mSettingsData.bumpWeight;
    settingsData.useShadowMap = mShadowMap != nullptr && mSettingsData.useShadowMap > 0;
    settingsData.depthBias = mSettingsData.depthBias;
    mSettingsBuffer.Update(settingsData);

    const MathC::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const MathC::Matrix4 matView = mCamera->GetViewMatrix();
    const MathC::Matrix4 matProj = mCamera->GetProjectionMatrix();

    MathC::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData transformData;
    transformData.wvp = MathC::Transpose(matFinal);
    transformData.world = MathC::Transpose(matWorld);
    transformData.viewPosition = mCamera->GetPosition();
    if (settingsData.useShadowMap > 0)
    {
        const MathC::Matrix4 matLightView = mLightCamera->GetViewMatrix();
        const MathC::Matrix4 matLightProj = mLightCamera->GetProjectionMatrix();
        transformData.lwvp = MathC::Transpose(matWorld * matLightView * matLightProj);

        mShadowMap->BindPS(4);
    }

    mTransformBuffer.Update(transformData);

    mLightBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    TextureManager* tm = TextureManager::Get();
    tm->BindPS(renderObject.diffuseMapID, 0);
    tm->BindPS(renderObject.normalMapID, 1);
    tm->BindPS(renderObject.specularMapID, 2);
    tm->BindVS(renderObject.bumpMapID, 3);

    renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void StandardEffect::SetLightCamera(const Camera& camera)
{
    mLightCamera = &camera;
}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
    mDirectionalLight = &directionalLight;
}

void StandardEffect::SetShadowMap(const Texture& shadowMap)
{
    mShadowMap = &shadowMap;
}

void StandardEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
        if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
            mSettingsData.useDiffuseMap = useDiffuseMap ? 1 : 0;

        bool useNormalMap = mSettingsData.useNormalMap > 0;
        if (ImGui::Checkbox("UseNormalMap", &useNormalMap))
            mSettingsData.useNormalMap = useNormalMap ? 1 : 0;

        bool useSpecularMap = mSettingsData.useSpecularMap > 0;
        if (ImGui::Checkbox("UseSpecularMap", &useSpecularMap))
            mSettingsData.useSpecularMap = useSpecularMap ? 1 : 0;

        bool useLighting = mSettingsData.useLighting > 0;
        if (ImGui::Checkbox("UseLight", &useLighting))
            mSettingsData.useLighting = useLighting ? 1 : 0;

        bool useBumpMap = mSettingsData.useBumpMap > 0;
        if (ImGui::Checkbox("UseBumpMap", &useBumpMap))
        {
            mSettingsData.useBumpMap = useBumpMap ? 1 : 0;
        }
        ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.01, 0.0f, 100.0f);

        bool useShadowMap = mSettingsData.useShadowMap > 0;
        if (ImGui::Checkbox("UseShadowMap", &useShadowMap))
            mSettingsData.useShadowMap = useShadowMap ? 1 : 0;

        ImGui::DragFloat("DepthBias", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");
    }
}