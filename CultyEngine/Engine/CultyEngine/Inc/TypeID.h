#pragma once

namespace CultyEngine
{
    enum class ComponentID
    {
        Invalid,    // default value
        Animator,   // manages and control animations
        Camera,     // contain a Camera for viewing
        CameraFPS,  // move Camera with FPS controller
        Mesh,       // creates a mesh for render object
        Model,      // creates a model render object
        Rigidbody,  // physics object that gets added to the physics world
        SoundBank,  // adds an array of sound effects to an object
        SoundEffect,// adds a single sound effect to an object
        Transform,  // transform component for location data
        Count
    };

    enum class ServiceID
    {
        Invalid,
        Camera,
        Render,
        Physics,
        Count
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeID() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypeID() const override { return StaticGetTypeID(); }