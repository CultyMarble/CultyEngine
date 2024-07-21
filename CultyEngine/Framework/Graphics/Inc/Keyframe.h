#pragma once
#include "Common.h"

namespace CultyEngine::Graphics
{
	enum class EaseType
	{
		Linear,
		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,
	};

	template<class T>
	struct Keyframe
	{
		Keyframe() = default;
		Keyframe(const T& k, float t, EaseType e = EaseType::Linear)
			: key(k), time(t), easeType(e) {}

		T key = T();
		float time = 0.0f;
		EaseType easeType = EaseType::Linear;
	};

	template<class T>
	using Keyframes = std::vector<Keyframe<T>>;

	using PositionKeys = Keyframes<MathC::Vector3>;
	using RotationKeys = Keyframes<MathC::Quaternion>;
	using ScaleKeys = Keyframes<MathC::Vector3>; 
}