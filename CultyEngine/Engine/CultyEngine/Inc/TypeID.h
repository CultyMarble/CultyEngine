#pragma once

namespace CultyEngine
{
    enum class ComponentID
    {
        Invalid,    // default value
        Transform,  // transform component for location data
        Camera,     // contain a Camera for viewing
        CameraFPS,  // move Camera with FPS controller
        Mesh,       // creates a mesh for render object
        Model,      // creates a model render object
        Count
    };

    enum class ServiceID
    {
        Invalid,
        Camera,
        Render,
        Count
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeID() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypeID() const override { return StaticGetTypeID(); }