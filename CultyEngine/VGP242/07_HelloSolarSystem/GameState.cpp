#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

namespace
{
    bool showGrid = true;
    float gridAlpha = 0.5f;
    Vector4 gridColor = CultyEngine::Colors::White;

    CultyEngine::MathC::Vector3 transformPos = CultyEngine::MathC::Vector3::Zero;

    bool drawMercury = true;
    bool drawVenus = true;
    bool drawEarth = true;
    bool drawMars = true;
    bool drawJupiter = true;
    bool drawSaturn = true;
    bool drawUranus = true;
    bool drawNeptune = true;
    bool drawPluto = true;

    const float RING_MERCURY =  15.0f;
    const float RING_VENUS =    27.7f;
    const float RING_EARTH =    38.5f;
    const float RING_MARS =     58.5f;
    const float RING_JUPITER =  86.0f;
    const float RING_SATURN =   129.3f;
    const float RING_URANUS =   179.5f;
    const float RING_NEPTUNE =  235.9f;
    const float RING_PLUTO =    312.5f;

    const float REVOLVE_MERCURY =   4.787f;
    const float REVOLVE_VENUS =     3.502f;
    const float REVOLVE_EARTH =     0.100f;
    const float REVOLVE_MARS =      2.407f;
    const float REVOLVE_JUPITER =   1.307f;
    const float REVOLVE_SATURN =    0.969f;
    const float REVOLVE_URANUS =    0.681f;
    const float REVOLVE_NEPTUNE =   0.543f;
    const float REVOLVE_PLUTO =     0.470f;

    const float ROTATE_MERCURY =    3.750f;
    const float ROTATE_VENUS =      -0.02f;
    const float ROTATE_EARTH =      2.000f;
    const float ROTATE_MARS =       0.241f;
    const float ROTATE_JUPITER =    12.59f;
    const float ROTATE_SATURN =     10.24f;
    const float ROTATE_URANUS =     2.540f;
    const float ROTATE_NEPTUNE =    2.530f;
    const float ROTATE_PLUTO =      0.333f;

    float revolveMercury = REVOLVE_MERCURY;
    float revolveVenus = REVOLVE_VENUS;
    float revolveEarth = REVOLVE_EARTH;
    float revolveMars = REVOLVE_MARS;
    float revolveJupiter = REVOLVE_JUPITER;
    float revolveSaturn = REVOLVE_SATURN;
    float revolveUranus = REVOLVE_URANUS;
    float revolveNeptune = REVOLVE_NEPTUNE;
    float revolvePluto = REVOLVE_PLUTO;

    float rotateMercury = ROTATE_MERCURY;
    float rotateVenus = ROTATE_VENUS;
    float rotateEarth = ROTATE_EARTH;
    float rotateMars = ROTATE_MARS;
    float rotateJupiter = ROTATE_JUPITER;
    float rotateSaturn = ROTATE_SATURN;
    float rotateUranus = ROTATE_URANUS;
    float rotateNeptune = ROTATE_NEPTUNE;
    float rotatePluto = ROTATE_PLUTO;

    void CameraControl(float deltaTime, Camera& mCamera)
    {
        auto input = Input::InputSystem::Get();
        const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 50.0f;
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

GameState::~GameState()
{
    delete(sun);
    delete(mercury);
    delete(venus);
    delete(earth);
    delete(mars);
    delete(jupiter);
    delete(saturn);
    delete(uranus);
    delete(neptune);
    delete(pluto);
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 50.0f, -100.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mGalaxyBG = MeshBuilder::CreateSkySpherePX(1000, 1000, 1000.0f);
    mGalaxyTexture.Initialize(L"../../Assets/Images/skysphere/space.jpg");
    mMeshBuffer.Initialize(mGalaxyBG);

    // Set up Shader Data
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";

    mConstantBuffer.Initialize(sizeof(MathC::Matrix4));
    mVertexShader.Initialize<VertexPX>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    sun =       new Planet(L"../../Assets/Images/planets/sun.jpg",          10.00f, 00.0f);
    mercury =   new Planet(L"../../Assets/Images/planets/mercury.jpg",      0.349f, 15.0f);
    venus =     new Planet(L"../../Assets/Images/planets/venus.jpg",        0.864f, 27.7f);
    earth =     new Planet(L"../../Assets/Images/planets/earth/earth.jpg",  0.910f, 38.5f);
    mars =      new Planet(L"../../Assets/Images/planets/mars.jpg",         0.485f, 58.5f);
    jupiter =   new Planet(L"../../Assets/Images/planets/jupiter.jpg",      9.987f, 86.0f);
    saturn =    new Planet(L"../../Assets/Images/planets/saturn.jpg",       8.319f, 129.3f);
    uranus =    new Planet(L"../../Assets/Images/planets/uranus.jpg",       3.623f, 179.5f);
    neptune =   new Planet(L"../../Assets/Images/planets/neptune.jpg",      3.517f, 235.9f);
    pluto =     new Planet(L"../../Assets/Images/planets/pluto.jpg",        0.295f, 312.5f);
}

void GameState::Terminate()
{
    mGalaxyTexture.Terminate();
    mMeshBuffer.Terminate();

    mSampler.Terminate();

    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();

    // Terminate Planet Data
    mercury->Terminate();
    sun->Terminate();
    venus->Terminate();
    earth->Terminate();
    mars->Terminate();
    jupiter->Terminate();
    saturn->Terminate();
    uranus->Terminate();
    neptune->Terminate();
    pluto->Terminate();
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    sun->Update(deltaTime, 0.0f, 0.1f);

    mercury->Update(deltaTime, revolveMercury, rotateMercury);
    venus->Update(deltaTime, revolveVenus, rotateVenus);
    earth->Update(deltaTime, revolveEarth, rotateEarth);
    mars->Update(deltaTime, revolveMars, rotateMars);
    jupiter->Update(deltaTime, revolveJupiter, rotateJupiter);
    saturn->Update(deltaTime, revolveSaturn, rotateSaturn);
    uranus->Update(deltaTime, revolveUranus, rotateUranus);
    neptune->Update(deltaTime, revolveNeptune, rotateNeptune);
    pluto->Update(deltaTime, revolvePluto, rotatePluto);
}

void GameState::Render()
{
    // Bind
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    // Render Galaxy BG
    mGalaxyTexture.BindPS(0);

    MathC::Matrix4 matWorld = MathC::Matrix4::Identity;
    MathC::Matrix4 matView = mCamera.GetViewMatrix();
    MathC::Matrix4 matProjection = mCamera.GetProjectionMatrix();
    MathC::Matrix4 matFinal = matWorld * matView * matProjection;
    MathC::Matrix4 wvp = MathC::Transpose(matFinal);

    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    mMeshBuffer.Render();

    // Render each planet
    sun->Render(mCamera, mConstantBuffer);

    if (drawMercury)
        mercury->Render(mCamera, mConstantBuffer);
    if (drawVenus)
        venus->Render(mCamera, mConstantBuffer);
    if (drawEarth)
        earth->Render(mCamera, mConstantBuffer);
    if (drawMars)
        mars->Render(mCamera, mConstantBuffer);
    if (drawJupiter)
        jupiter->Render(mCamera, mConstantBuffer);
    if (drawSaturn)
        saturn->Render(mCamera, mConstantBuffer);
    if (drawUranus)
        uranus->Render(mCamera, mConstantBuffer);
    if (drawNeptune)
        neptune->Render(mCamera, mConstantBuffer);
    if (drawPluto)
        pluto->Render(mCamera, mConstantBuffer);
}

void GameState::DebugUI()
{
    ImGui::Begin("DebugUI: Hello Solar System", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        DebugUI::SetTheme(DebugUI::Theme::Dark);

        if (ImGui::CollapsingHeader("Gizmo", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Show Grid", &showGrid);
            ImGui::DragFloat("Grid Alpha", &gridAlpha, 0.001f, 0.0f, 1.0f);
        }

        if (ImGui::CollapsingHeader("Planet Configs", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::Button("Reset Configs"))
            {
                rotateMercury = ROTATE_MERCURY; revolveMercury = REVOLVE_MERCURY;
                rotateVenus = ROTATE_VENUS; revolveVenus = REVOLVE_VENUS;
                rotateEarth = ROTATE_EARTH; revolveEarth = REVOLVE_EARTH;
                rotateMars = ROTATE_MARS; revolveMars = REVOLVE_MARS;
                rotateJupiter = ROTATE_JUPITER; revolveJupiter = REVOLVE_JUPITER;
                rotateSaturn = ROTATE_SATURN; revolveSaturn = REVOLVE_SATURN;
                rotateUranus = ROTATE_URANUS; revolveUranus = REVOLVE_URANUS;
                rotateNeptune = ROTATE_NEPTUNE; revolveNeptune = REVOLVE_NEPTUNE;
                rotatePluto = ROTATE_PLUTO; revolvePluto = REVOLVE_PLUTO;
            }

            if (ImGui::CollapsingHeader("MERCURY", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawMercury);
                ImGui::DragFloat("Rotate Speed", &rotateMercury, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolveMercury, 0.05f, 0.0f, 50.0f);
            }

            if (ImGui::CollapsingHeader("VENUS", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawVenus);
                ImGui::DragFloat("Rotate Speed", &rotateVenus, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolveVenus, 0.05f, 0.0f, 50.0f);
            }

            if (ImGui::CollapsingHeader("EARTH", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawEarth);
                ImGui::DragFloat("Rotate Speed", &rotateEarth, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolveEarth, 0.05f, 0.0f, 50.0f);
            }

            if (ImGui::CollapsingHeader("MARS", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawMars);
                ImGui::DragFloat("Rotate Speed", &rotateMars, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolveMars, 0.05f, 0.0f, 50.0f);
            }

            if (ImGui::CollapsingHeader("JUPITER", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawJupiter);
                ImGui::DragFloat("Rotate Speed", &rotateJupiter, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolveJupiter, 0.05f, 0.0f, 50.0f);
            }

            if (ImGui::CollapsingHeader("SATURN", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawSaturn);
                ImGui::DragFloat("Rotate Speed", &rotateSaturn, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolveSaturn, 0.05f, 0.0f, 50.0f);
            }

            if (ImGui::CollapsingHeader("URANUS", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawUranus);
                ImGui::DragFloat("Rotate Speed", &rotateUranus, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolveUranus, 0.05f, 0.0f, 50.0f);
            }

            if (ImGui::CollapsingHeader("NEPTUNE", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawNeptune);
                ImGui::DragFloat("Rotate Speed", &rotateNeptune, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolveNeptune, 0.05f, 0.0f, 50.0f);
            }

            if (ImGui::CollapsingHeader("PLUTO", ImGuiTreeNodeFlags_CollapsingHeader))
            {
                ImGui::Checkbox("Draw", &drawPluto);
                ImGui::DragFloat("Rotate Speed", &rotatePluto, 0.05f, 0.0f, 50.0f);
                ImGui::DragFloat("Revolve Speed", &revolvePluto, 0.05f, 0.0f, 50.0f);
            }
        }

    ImGui::End();

    if (showGrid)
    {
        gridColor.a = gridAlpha;

        SimpleDraw::AddGroundCircle(1000, RING_MERCURY, gridColor);
        SimpleDraw::AddGroundCircle(1000, RING_VENUS, gridColor);
        SimpleDraw::AddGroundCircle(1000, RING_EARTH, gridColor);
        SimpleDraw::AddGroundCircle(1000, RING_MARS, gridColor);
        SimpleDraw::AddGroundCircle(1000, RING_JUPITER, gridColor);
        SimpleDraw::AddGroundCircle(1000, RING_SATURN, gridColor);
        SimpleDraw::AddGroundCircle(1000, RING_URANUS, gridColor);
        SimpleDraw::AddGroundCircle(1000, RING_NEPTUNE, gridColor);
        SimpleDraw::AddGroundCircle(1000, RING_PLUTO, gridColor);
    }

    SimpleDraw::Render(mCamera);
}