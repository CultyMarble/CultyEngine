#pragma once
#include <CultyEngine/Inc/CultyEngine.h>

class GameState : public CultyEngine::ApplicationState
{
public:
    void Initialize() override;

    void Terminate() override;

    void Update(float deltaTime) override;

    void Render() override;

protected:
    virtual void CreateShapeData() {};

    struct Vertex
    {
        CultyEngine::MathC::Vector3 position;
        CultyEngine::Color color;
    };

    using Vertices = std::vector<Vertex>;
    Vertices mVertices;

    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;
};

class TriforceSymbolState : public GameState
{
public:
    void Update(float deltaTime) override;

protected:
    void CreateShapeData() override;
};

class DiamondState : public GameState
{
public:
    void Update(float deltaTime) override;

protected:
    void CreateShapeData() override;
};

class HeartState : public GameState
{
public:
    void Update(float deltaTime) override;

protected:
    void CreateShapeData() override;
};