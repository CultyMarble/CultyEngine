#pragma once
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace CultyEngine::Graphics
{
    class Camera;
    class Texture;
    struct RenderObject;

    class TerrainEffect final
    {
    public:
        void Initialize();
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);
        void DebugUI();

        void SetCamera(const Camera& camera);
        void SetLightCamera(const Camera& camera);
        void SetDirectionalLight(const DirectionalLight& directionalLight);
        void SetShadowMap(const Texture& shadowMap);

    private:
        struct TransformData
        {
            MathC::Matrix4 wvp;
            MathC::Matrix4 lwvp;
            MathC::Matrix4 world;
            MathC::Vector3 viewPosition;
            float padding = 0.0f;
        };

        struct SettingsData
        {
            int useNormalMap = 1;
            int useSpecularMap = 1;
            int useShadowMap = 1;
            int useBlend = 1;
            float depthBias = 0.000001f;
            float blendHeight = 0.0f;
            float blendWidth = 0.0f;
            float padding = 0.0f;
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        using MaterialBuffer = TypedConstantBuffer<Material>;

        TransformBuffer mTransformBuffer;
        SettingsBuffer mSettingsBuffer;
        LightBuffer mLightBuffer;
        MaterialBuffer mMaterialBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;
        Sampler mSampler;

        SettingsData mSettingsData;
        const Camera* mCamera = nullptr;
        const Camera* mLightCamera = nullptr;
        const DirectionalLight* mDirectionalLight = nullptr;
        const Texture* mShadowMap = nullptr;
    };
}