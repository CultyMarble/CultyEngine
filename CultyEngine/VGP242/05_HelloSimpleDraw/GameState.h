#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

class GameState : public CultyEngine::ApplicationState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    // virtual void Render() override;

protected:
    CultyEngine::Graphics::Camera mCamera;
};

class StateTransform : public GameState
{
public:
    void Render() override;
};

class StateGroundPlane : public GameState
{
public:
    void Render() override;
};

class StateSphere : public GameState
{
public:
    void Render() override;
};

class StateAABB : public GameState
{
public:
    void Render() override;
};

class StateFilledAABB : public GameState
{
public:
    void Render() override;
};

class StateLines : public GameState
{
public:
    void Render() override;
};