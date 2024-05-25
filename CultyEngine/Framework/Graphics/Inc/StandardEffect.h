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
    struct RenderObject;

    class StandardEffect
    {
    public:
        void Initialize(const std::filesystem::path& filePath);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);

        void SetCamera(const Camera& camera);
        void SetDirectionalLight(const DirectionalLight& directionalLight);
        void SetShadowMap(const Texture& shadowMap);

        void DebugUI();

    private:
        struct TransformData
        {
            MathC::Matrix4 wvp;
            MathC::Matrix4 world;
            MathC::Vector3 viewPosition;
            float padding = 0.0f;
        };

        struct SettingsData
        {
            int useDiffuseMap = 1;
            int useNormalMap = 1;
            int useSpecularMap = 1;
            int useLighting = 1;
            int useBumpMap = 1;
            int useShadowMap = 1;

            float bumpWeight = 1.0f;
            float depthBias = 0.0f;
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        using MaterialBuffer = TypedConstantBuffer<Material>;

        TransformBuffer mTransformBuffer;
        SettingsBuffer mSettingsBuffer;
        LightBuffer mLightBuffer;
        MaterialBuffer mMaterialBuffer;

        Sampler mSampler;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        SettingsData mSettingsData;
        const Camera* mCamera = nullptr;
        const DirectionalLight* mDirectionalLight = nullptr;
    };
}