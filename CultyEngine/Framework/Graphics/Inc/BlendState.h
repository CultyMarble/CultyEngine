#pragma once

namespace CultyEngine::Graphics
{
	class BlendState final
	{
	public:
		static void ClearState();

		enum class BlendMode
		{
			Additive,
			AlphaBlend,
			AlphaPremultiplied,
			Opaque,
		};

		BlendState() = default;
		~BlendState();

		BlendState(const BlendState&) = delete;
		BlendState& operator=(const BlendState&) = delete;

		void Initialize(BlendMode mode);
		void Terminate();

		void Set();

	private:
		ID3D11BlendState* mBlendState = nullptr;
	};
}
