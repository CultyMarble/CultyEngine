#pragma once
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace CultyEngine::Graphics
{
	struct RenderObject;
	class Texture;

	class CRTEffect
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void Update(float deltaTime);

		void DebugUI();

		void SetTexture(const Texture* texture, uint32_t slot = 0);

	private:
		struct PostProcessData
		{
			float pixelationSize = 0.0f;

			float edgeDistortion = 0.0f;

			float abberationSeperation = 0.0f;

			float scanlineFrequency = 0.0f;
			float scanlineBrightness = 0.0f;
			float scanlineAlpha = 0.0f;
			float scanlineOffsetY = 0.0f;

			float bloomThreshold = 0.0f;
			float bloomStrength = 0.0f;

			float noiseStrength = 0.0f;
			float noiseAlpha = 0.0f;
			float noiseRandom = 0.0f;

			float jitterThickness = 0.0f;
			float jitterStrength = 0.0f;
			float jitterRandom = 0.0f;

			float breakFrequency = 0.0f;
			float breakStrength = 0.0f;

			float VHSoverlayStrength = 0.0f;
			float VHSoverlayWidth = 0.0f;

			float padding = 0.0f;
		};

		using PostProcessingBuffer = TypedConstantBuffer<PostProcessData>;
		PostProcessingBuffer mPostProcessingBuffer;

		Sampler mSampler;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		std::array<const Texture*, 4> mTextures;

		float mTick = 0.0f;
		float mPixelationSize = 450.0f;
		float mEdgeDistortion = 0.5f;
		float mAberrationSeperation = 0.005f;
		int mScanlineFrequency = 400;
        float mScanlineBrightness = 0.75f;
        float mScanlineAlpha = 0.075f;
		float mScanlineOffsetY = 0.0f;
		float mBloomThreshold = 0.1f;
		float mBloomStrength = 0.5f;
        float mNoiseStrength = 0.25f;
        float mNoiseAlpha = 0.25f;
        float mNoiseRandom = 0.0f;
        float mJitterThickness = 0.5f;
        float mJitterStrength = 0.002f;
        float mJitterRandom = 10.0f;
		float mBreakFrequency = 0.0f;
		float mBreakFrequencyTick = 0.0f;
		float mBreakStrength = 0.01f;
		float mVHSOverlayStrength = 1.0f;
		float mVHSOverlayWidth = 0.25f;
	};
}