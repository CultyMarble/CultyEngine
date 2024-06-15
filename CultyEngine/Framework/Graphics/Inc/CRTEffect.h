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
			float params0 = 0.0f; // Abberation Seperation

			float params1 = 0.0f; // Scanline Frequency
			float params2 = 0.0f; // Scanline Brightness
			float params3 = 0.0f; // Scanline Aplha
			float params4 = 0.0f; // Scanline Speed

			float params5 = 0.0f; // Bloom Threshold
			float params6 = 0.0f; // Bloom Strength

			float params7 = 0.0f; // Noise Brightness
			float params8 = 0.0f; // Noise Alpha
			float params9 = 0.0f; // Noise Random

			float params10 = 0.0f; // Jitter Check
			float params11 = 0.0f; // Jitter Strength
			float params12 = 0.0f; // Jitter Random

            float padding[3] = { 0.0f };
		};

		using PostProcessingBuffer = TypedConstantBuffer<PostProcessData>;
		PostProcessingBuffer mPostProcessingBuffer;

		Sampler mSampler;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		std::array<const Texture*, 4> mTextures;

		float mAberrationSeperation = 0.005f;
		
		int mScanlineFrequency = 400;
        float mScanlineBrightness = 0.75f;
        float mScanlineAlpha = 0.075f;
		float mScanlineOffsetY = 0.0f;

		float mBloomThreshold = 0.8f;
		float mBloomStrength = 4.0f;

        float mNoiseBrightness = 0.25f;
        float mNoiseAlpha = 0.25f;
        float mNoiseRandom = 0.0f;

        float mJitterThickness = 0.5f;
        float mJitterStrength = 0.002f;
        float mJitterRandom = 10.0f;
	};
}