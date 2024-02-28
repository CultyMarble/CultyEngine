#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Input;

void GameState::Initialize()
{
    // Create a shape data
    CreateShapeData();

    auto device = CultyEngine::Graphics::GraphicsSystem::Get()->GetDevice();

    // Create a way to send data to GPU
    // We need a vertex buffer
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size()) * sizeof(Vertex);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = mVertices.data();

    HRESULT hResult = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
    ASSERT(SUCCEEDED(hResult), "Failed to created vertex data!");

    //===========================================================================
    // Need to create a vertex shader
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoSomething.fx";

    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    hResult = D3DCompileFromFile(
        shaderFilePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0",
        shaderFlags, 0,
        &shaderBlob,
        &errorBlob
    );

    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    }
    ASSERT(SUCCEEDED(hResult), "Failed to compile vertex shader!");

    hResult = device->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mVertexShader
    );
    ASSERT(SUCCEEDED(hResult), "Failed to create vertex shader!");

    //===========================================================================
    // Need to create input layout
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
    vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });
    vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });

    hResult = device->CreateInputLayout(
        vertexLayout.data(),
        static_cast<UINT>(vertexLayout.size()),
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        &mInputLayout
    );

    ASSERT(SUCCEEDED(hResult), "Failed to create input layout!");
    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);

    //===========================================================================
    // Need to create a pixel shader
    hResult = D3DCompileFromFile(
        shaderFilePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS", "ps_5_0",
        shaderFlags, 0,
        &shaderBlob,
        &errorBlob
    );

    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    }
    ASSERT(SUCCEEDED(hResult), "Failed to compile pixel shader!");

    hResult = device->CreatePixelShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mPixelShader
    );

    ASSERT(SUCCEEDED(hResult), "Failed to create pixel shader!");
    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
}

void GameState::Terminate()
{
    LOG("GAME STATE TERMINATED!");

    mVertices.clear();
    SafeRelease(mPixelShader);
    SafeRelease(mInputLayout);
    SafeRelease(mVertexShader);
    SafeRelease(mVertexBuffer);
}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
    auto context = CultyEngine::Graphics::GraphicsSystem::Get()->GetContext();

    // binde
    context->VSSetShader(mVertexShader, nullptr, 0);
    context->IASetInputLayout(mInputLayout);
    context->PSSetShader(mPixelShader, nullptr, 0);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    context->Draw(static_cast<UINT>(mVertices.size()), 0);
}

// TRIFORCE SYMBOL
void TriforceSymbolState::Update(float deltaTime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApplication().ChangeState("DiamondState");
    }
    else if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApplication().ChangeState("HeartState");
    }
}

void TriforceSymbolState::CreateShapeData()
{
    // TOP TRI
    mVertices.push_back({ { -0.17f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, CultyEngine::Colors::Yellow });
    mVertices.push_back({ { 0.17f, 0.25f, 0.0f }, CultyEngine::Colors::Blue });

    // LEFT TRI
    mVertices.push_back({ { -0.34f, -0.25f, 0.0f }, CultyEngine::Colors::Yellow });
    mVertices.push_back({ { -0.17f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { 0.0f, -0.25f, 0.0f }, CultyEngine::Colors::Green });

    // RIGHT TRI
    mVertices.push_back({ { 0.0f, -0.25f, 0.0f }, CultyEngine::Colors::Green });
    mVertices.push_back({ { 0.17f, 0.25f, 0.0f }, CultyEngine::Colors::Blue });
    mVertices.push_back({ { 0.34f, -0.25f, 0.0f }, CultyEngine::Colors::Yellow });
}

// DIAMOND
void DiamondState::Update(float deltaTime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApplication().ChangeState("TriforceSymbolState");
    }
    else if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApplication().ChangeState("HeartState");
    }
}

void DiamondState::CreateShapeData()
{
    mVertices.push_back({ { -0.3f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { -0.15f, 0.6f, 0.0f }, CultyEngine::Colors::Yellow });
    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::White });

    mVertices.push_back({ { -0.15f, 0.6f, 0.0f }, CultyEngine::Colors::Yellow });
    mVertices.push_back({ { 0.15f, 0.6f, 0.0f }, CultyEngine::Colors::Green });
    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::White });

    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::White });
    mVertices.push_back({ { 0.15f, 0.6f, 0.0f }, CultyEngine::Colors::Green });
    mVertices.push_back({ { 0.3f, 0.25f, 0.0f }, CultyEngine::Colors::Blue });

    mVertices.push_back({ { 0.0f, -0.4f, 0.0f }, CultyEngine::Colors::Purple });
    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::White });
    mVertices.push_back({ { 0.3f, 0.25f, 0.0f }, CultyEngine::Colors::Blue });

    mVertices.push_back({ { -0.3f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::White });
    mVertices.push_back({ { 0.0f, -0.4f, 0.0f }, CultyEngine::Colors::Purple });
}

// HEART
void HeartState::Update(float deltaTime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApplication().ChangeState("TriforceSymbolState");
    }
    else if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApplication().ChangeState("DiamondState");
    }
}

void HeartState::CreateShapeData()
{
    mVertices.push_back({ { -0.4f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { -0.3f, 0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { -0.2f, 0.25f, 0.0f }, CultyEngine::Colors::Red });

    mVertices.push_back({ { -0.3f, 0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { -0.1f, 0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { -0.2f, 0.25f, 0.0f }, CultyEngine::Colors::Red });

    mVertices.push_back({ { -0.2f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { -0.1f, 0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::Red });

    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { 0.1f, 0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { 0.2f, 0.25f, 0.0f }, CultyEngine::Colors::Red });

    mVertices.push_back({ { 0.1f, 0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { 0.3f, 0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { 0.2f, 0.25f, 0.0f }, CultyEngine::Colors::Red });

    mVertices.push_back({ { 0.2f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { 0.3f, 0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { 0.4f, 0.25f, 0.0f }, CultyEngine::Colors::Red });

    mVertices.push_back({ { 0.0f, -0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { 0.4f, 0.25f, 0.0f }, CultyEngine::Colors::Red });

    mVertices.push_back({ { -0.4f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { 0.0f, 0.25f, 0.0f }, CultyEngine::Colors::Red });
    mVertices.push_back({ { 0.0f, -0.5f, 0.0f }, CultyEngine::Colors::DeepPink });
}