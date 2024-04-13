#pragma once
#include "ConstantBuffer.h"
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

		void DebugUI();

	private:
		struct TransformData
		{
			MathC::Matrix4 wvp;
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;
			float padding[3] = { 0 };
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		TransformBuffer mTransformBuffer;
		SettingsBuffer mSettingsBuffer;

		Sampler mSampler;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const Camera* mCamera = nullptr;
		SettingsData mSettingsData;
	};
}