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
			MathC::Matrix4 viewPosition;
			float padding = 0.0f;
		};

		struct SettingsData
		{
			int useNormalMap = 0;
			int useSpecularMap = 0;
			int useShadowMap = 0;
			int useBlend = 0;
			float depthBias = 0.00001f;
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
		const 
	};
}