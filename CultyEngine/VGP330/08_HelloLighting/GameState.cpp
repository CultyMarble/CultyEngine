#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

namespace
{
    void CameraControl(float deltaTime, Camera& mCamera)
    {
        auto input = Input::InputSystem::Get();
        const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
        const float turnSpeed = 0.1f;

        if (input->IsKeyDown(KeyCode::W))
            mCamera.Walk(moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::S))
            mCamera.Walk(-moveSpeed * deltaTime);

        if (input->IsKeyDown(KeyCode::A))
            mCamera.Strafe(-moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::D))
            mCamera.Strafe(moveSpeed * deltaTime);

        if (input->IsKeyDown(KeyCode::Q))
            mCamera.Rise(-moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::E))
            mCamera.Rise(moveSpeed * deltaTime);

        if (input->IsMouseDown(MouseButton::RBUTTON))
        {
            mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
            mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
        }
    }
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    // create a shape
    TextureManager* tm = TextureManager::Get();
    mMesh = MeshBuilder::CreateSphere(100, 100, 1.0f);

    mCharacter_01.meshBuffer.Initialize(mMesh);
    mCharacter_01.diffuseMapID = tm->LoadTexture("planets/earth/earth.jpg");
    mCharacter_01.normalMapID = tm->LoadTexture("planets/earth/earth_normal.jpg");
    mCharacter_01.specularMapID = tm->LoadTexture("planets/earth/earth_spec.jpg");
    mCharacter_01.bumpMapID = tm->LoadTexture("planets/earth/earth_bump.jpg");
    mCharacter_01.transform.position.x = 0.0f;

    //mRenderObject2.meshBuffer.Initialize(mMesh);
    //mRenderObject2.diffuseMapID = tm->LoadTexture("download/mars_diffuse.jpg");
    //mRenderObject2.specularMapID = tm->LoadTexture("download/mars_spec.jpg");
    //mRenderObject2.normalMapID = tm->LoadTexture("download/mars_normal.jpg");
    //mRenderObject2.transform.position.x = 2.0f;

    //int numberOfObjects = 1;
    //for (int i = 0; i < numberOfObjects; ++i)
    //{
    //    RenderObject& newObject = mRenderObject.emplace_back();

    //    newObject.meshBuffer.Initialize(mMesh);

    //    newObject.diffuseMapID = tm->LoadTexture("planets/earth/earth.jpg");
    //    newObject.specularMapID = tm->LoadTexture("planets/earth/earth_spec.jpg");
    //    newObject.normalMapID = tm->LoadTexture("planets/earth/earth_normal.jpg");

    //    newObject.transform.position.x = float(i);
    //}
    
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();

    //mRenderObject2.Terminate();
    mCharacter_01.Terminate();

    //for (auto& r : mRenderObject)
    //    r.Terminate();
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        mStandardEffect.Render(mCharacter_01);
        //mStandardEffect.Render(mRenderObject2);

        //for (auto& r : mRenderObject)
        //    mStandardEffect.Render(r);

    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
            {
                mDirectionalLight.direction = MathC::Normalize(mDirectionalLight.direction);
            }

            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }

        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_CollapsingHeader))
        {
            ImGui::ColorEdit4("Ambient##Material", &mCharacter_01.material.ambient.r);
            ImGui::ColorEdit4("Diffuse##Material", &mCharacter_01.material.diffuse.r);
            ImGui::ColorEdit4("Specular##Material", &mCharacter_01.material.specular.r);
            ImGui::ColorEdit4("Emissive##Material", &mCharacter_01.material.emissive.r);

            ImGui::DragFloat("SpecularPower##Material", &mCharacter_01.material.power, 1.0f, 1.0f, 500.0f);
        }
        mStandardEffect.DebugUI();
    ImGui::End();
}