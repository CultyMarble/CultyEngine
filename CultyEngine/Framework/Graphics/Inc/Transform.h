#pragma once
#include "Common.h"

namespace CultyEngine::Graphics
{
	struct Transform
	{
		MathC::Vector3 position = MathC::Vector3::Zero;
		MathC::Quaternion rotation = MathC::Quaternion::Identity;
		MathC::Vector3 scale = MathC::Vector3::One;

		MathC::Matrix4 GetMatrix4() const
		{
			return {
				MathC::Matrix4::Scaling(scale) *
				MathC::Matrix4::MatrixRotationQuaternion(rotation) *
				MathC::Matrix4::Translation(position)
			};
		}
	};
}
