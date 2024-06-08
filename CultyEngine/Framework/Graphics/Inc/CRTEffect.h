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
			float params0 = 0.0f;
			float params1 = 0.0f;
			float params2 = 0.0f;
			float padding = 0.0f;
		};

		using PostProcessingBuffer = TypedConstantBuffer<PostProcessData>;
		PostProcessingBuffer mPostProcessingBuffer;

		Sampler mSampler;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		std::array<const Texture*, 4> mTextures;

		float mAberrationValue = 0.01f;
		
		int mScanLine = 500;
		float mUVOffsetY = 0.0f;
	};
}