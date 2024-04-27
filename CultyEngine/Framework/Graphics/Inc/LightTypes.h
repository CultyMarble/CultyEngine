#pragma once
#include "Colors.h"

namespace CultyEngine::Graphics
{
    struct DirectionalLight
    {
        Color ambient = Colors::White;
        Color diffuse = Colors::White;
        Color specular = Colors::White;
        MathC::Vector3 direction = -MathC::Vector3::YAxis; // pointing down
        float padding = 0.0f;
    };

    struct PointLight
    {

    };

    struct SpotLight
    {

    };
}