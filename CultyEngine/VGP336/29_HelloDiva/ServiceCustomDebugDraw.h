#pragma once
#include "CustomTypeID.h"

class ComponentCustomDebugDraw;

class ServiceCustomDebugDraw : public CultyEngine::Service
{
public:
    SET_TYPE_ID(CustomServiceID::CustomDebugDrawDisplay);

    void Render() override;

    void Register(ComponentCustomDebugDraw* componentDebugDraw);
    void Unregister(ComponentCustomDebugDraw* componentDebugDraw);

private:
    using DebugDrawComponents = std::vector<ComponentCustomDebugDraw*>;
    DebugDrawComponents mCustomDebugDrawComponents;
};