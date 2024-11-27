#pragma once
#include "Component.h"

namespace CultyEngine
{
    class ComponentUI : public Component
    {
    public:
        virtual void Render() = 0;
    };
}