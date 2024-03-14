#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] = {
            CultyEngine::Colors::Orchid,
            CultyEngine::Colors::DeepPink,
            CultyEngine::Colors::Chocolate,
            CultyEngine::Colors::GreenYellow,
            CultyEngine::Colors::Maroon,
            CultyEngine::Colors::MidnightBlue,
            CultyEngine::Colors::Wheat,
            CultyEngine::Colors::Khaki,
        };

        index = (index + 1) % std::size(colorTable);

        return colorTable[index];
    }

    void CreateCudeIndices(std::vector<uint32_t>& indices)
    {
        indices = {
            // front
            0, 1, 2, 0, 2, 3,
            // back
            7, 5, 4, 7, 6, 5,
            // right
            3, 2, 6, 3, 6, 7,
            // left
            4, 5, 1, 4, 1, 0,
            // top
            1, 5, 6, 1, 6, 2,
            // bottom
            0, 3, 7, 0, 7, 4,
        };
    }

    void CreatePlaneIndices(std::vector<uint32_t>& indices, uint32_t numRows, uint32_t numCols)
    {
        for (uint32_t r = 0; r < numRows; ++r)
        {
            for (uint32_t c = 0; c < numCols; ++c)
            {
                uint32_t i = (r * (numCols + 1)) + c;

                // triangle 1
                indices.push_back(i);
                indices.push_back(i + numCols + 2);
                indices.push_back(i + 1);

                // triangle 2
                indices.push_back(i);
                indices.push_back(i + numCols + 1);
                indices.push_back(i + numCols + 2);
            }
        }
    }
}

MeshPC MeshBuilder::CreatePyramidPC(float size)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int index = rand() & 100;

    MeshPC mesh;

    const float hs = size * 0.5f;

    // front
    mesh.vertices.push_back({ {-hs, -hs, -hs}, GetNextColor(index) });
    mesh.vertices.push_back({ {0.0f, hs, 0.0f}, GetNextColor(index) });
    mesh.vertices.push_back({ {hs, -hs, -hs}, GetNextColor(index) });

    // back
    mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { hs, -hs, hs }, GetNextColor(index) });

    // Indices
    mesh.indices = {
        // front
        0, 1, 2,
        // right
        2, 1, 4,
        // back
        4, 1, 3,
        // left
        0, 3, 1,
        // bottom
        0, 2, 4,
        0, 4, 3,
    };
    return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC meshPC;

    const float hs = size * 0.5f;

    // front
    meshPC.vertices.push_back({ {-hs, -hs, -hs}, GetNextColor(index) });
    meshPC.vertices.push_back({ {-hs, hs, -hs}, GetNextColor(index) });
    meshPC.vertices.push_back({ {hs, hs, -hs}, GetNextColor(index) });
    meshPC.vertices.push_back({ {hs, -hs, -hs}, GetNextColor(index) });

    // back
    meshPC.vertices.push_back({ {-hs, -hs, hs}, GetNextColor(index) });
    meshPC.vertices.push_back({ {-hs, hs, hs}, GetNextColor(index) });
    meshPC.vertices.push_back({ {hs, hs, hs}, GetNextColor(index) });
    meshPC.vertices.push_back({ {hs, -hs, hs}, GetNextColor(index) });

    CreateCudeIndices(meshPC.indices);

    return meshPC;
}

MeshPC MeshBuilder::CreateRectanglePC(float width, float height, float depth)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC meshPC;
    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    const float hd = depth * 0.5f;

    // front
    meshPC.vertices.push_back({ {-hw, -hh, -hd}, GetNextColor(index) });
    meshPC.vertices.push_back({ {-hw, hh, -hd}, GetNextColor(index) });
    meshPC.vertices.push_back({ {hw, hh, -hd}, GetNextColor(index) });
    meshPC.vertices.push_back({ {hw, -hh, -hd}, GetNextColor(index) });

    // back
    meshPC.vertices.push_back({ {-hw, -hh, hd}, GetNextColor(index) });
    meshPC.vertices.push_back({ {-hw, hh, hd}, GetNextColor(index) });
    meshPC.vertices.push_back({ {hw, hh, hd}, GetNextColor(index) });
    meshPC.vertices.push_back({ {hw, -hh, hd}, GetNextColor(index) });

    CreateCudeIndices(meshPC.indices);

    return meshPC;
}

MeshPC MeshBuilder::CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC meshPC;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float x = -hpw;
    float y = -hph;

    for (uint32_t r = 0; r <= numRows; ++r)
    {
        for (uint32_t c = 0; c <= numCols; ++c)
        {
            meshPC.vertices.push_back({ {x, y, 0.0f}, GetNextColor(index) });
            x += spacing;
        }
        x = -hpw;
        y += spacing;
    }

    CreatePlaneIndices(meshPC.indices, numRows, numCols);

    return meshPC;
}

MeshPX MeshBuilder::CreateVerticalPlanePX(uint32_t numRows, uint32_t numCols, float spacing)
{
    MeshPX mesh;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;
    const float uInc = 1.0f / static_cast<float>(numCols);
    const float vInc = 1.0f / static_cast<float>(numRows);

    float x = -hpw;
    float y = -hph;
    float u = 0.0f;
    float v = 1.0f;

    for (uint32_t r = 0; r <= numRows; ++r)
    {
        for (uint32_t c = 0; c <= numCols; ++c)
        {
            mesh.vertices.push_back({ {x, y, 0.0f}, {u, v} });
            x += spacing;
            u += uInc;
        }

        x = -hpw;
        y += spacing;
        u = 0.0f;
        v += (-vInc);
    }

    CreatePlaneIndices(mesh.indices, numRows, numCols);

    return mesh;
}

MeshPC MeshBuilder::CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC meshPC;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float x = -hpw;
    float z = -hph;

    for (uint32_t r = 0; r <= numRows; ++r)
    {
        for (uint32_t c = 0; c <= numCols; ++c)
        {
            meshPC.vertices.push_back({ {x, 0.0f, z}, GetNextColor(index) });
            x += spacing;
        }

        x = -hpw;
        z += spacing;
    }

    CreatePlaneIndices(meshPC.indices, numRows, numCols);

    return meshPC;
}

MeshPX MeshBuilder::CreateHorizontalPlanePX(uint32_t numRows, uint32_t numCols, float spacing)
{
    MeshPX meshPX;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;
    const float uInc = 1.0f / static_cast<float>(numCols);
    const float vInc = 1.0f / static_cast<float>(numRows);

    float x = -hpw;
    float z = -hph;
    float u = 0.0f;
    float v = 1.0f;

    for (uint32_t r = 0; r <= numRows; ++r)
    {
        for (uint32_t c = 0; c <= numCols; ++c)
        {
            meshPX.vertices.push_back({ {x, 0.0f, z}, {u, v} });
            x += spacing;
            u += uInc;
        }

        x = -hpw;
        z += spacing;
        u = 0.0f;
        v += (-vInc);
    }

    CreatePlaneIndices(meshPX.indices, numRows, numCols);

    return meshPX;
}

MeshPC MeshBuilder::CreateCylinderPC(uint32_t slices, uint32_t rings)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC meshPC;

    const float hh = static_cast<float>(rings) * 0.5f;

    for (uint32_t r = 0; r <= rings; ++r)
    {
        float ringPos = static_cast<float>(r);

        for (uint32_t s = 0; s <= slices; ++s)
        {
            float slicePos = static_cast<float>(s);
            float rotation = (slicePos / static_cast<float>(slices)) * MathC::Constants::TwoPi;

            meshPC.vertices.push_back({
                {cos(rotation), ringPos - hh, sin(rotation)},
                GetNextColor(index)
                });
        }
    }

    CreatePlaneIndices(meshPC.indices, rings, slices);

    // Top and Bottom cap
    meshPC.vertices.push_back({ {0.0f, hh, 0.0f}, GetNextColor(index) });
    meshPC.vertices.push_back({ {0.0f, -hh, 0.0f}, GetNextColor(index) });

    uint32_t bottomIndex = meshPC.vertices.size() - 1;
    uint32_t topIndex = bottomIndex - 1;

    for (uint32_t s = 0; s < slices; ++s)
    {
        // bottom triangle
        meshPC.indices.push_back(bottomIndex);
        meshPC.indices.push_back(s);
        meshPC.indices.push_back(s + 1);

        // top triangle
        uint32_t topRowIndex = topIndex - slices - 1 + s;
        meshPC.indices.push_back(topIndex);
        meshPC.indices.push_back(topRowIndex + 1);
        meshPC.indices.push_back(topRowIndex);
    }

    return meshPC;
}

MeshPC MeshBuilder::CreateSpherePC(uint32_t slices, uint32_t rings, float radius)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC meshPC;

    const float vertRotation = (MathC::Constants::Pi / static_cast<float>(rings));
    const float horzRotation = (MathC::Constants::TwoPi / static_cast<float>(slices));

    for (uint32_t r = 0; r <= rings; ++r)
    {
        float ringPos = static_cast<float>(r);
        float phi = ringPos * vertRotation;

        for (uint32_t s = 0; s <= rings; ++s)
        {
            float slicePos = static_cast<float>(s);
            float rotation = slicePos * horzRotation;

            meshPC.vertices.push_back({ {
                radius * sin(rotation) * sin(phi),
                radius * cos(phi),
                radius * cos(rotation) * sin(phi)},
                GetNextColor(index) });
        }
    }

    CreatePlaneIndices(meshPC.indices, rings, slices);

    return meshPC;
}

MeshPX MeshBuilder::CreateSpherePX(uint32_t slices, uint32_t rings, float radius)
{
    MeshPX meshPX;

    const float vertRotation = (MathC::Constants::Pi / static_cast<float>(rings));
    const float horzRotation = (MathC::Constants::TwoPi / static_cast<float>(slices));
    const float uInc = 1.0f / static_cast<float>(slices);
    const float vInc = 1.0f / static_cast<float>(rings);

    for (uint32_t r = 0; r <= rings; ++r)
    {
        float ringPos = static_cast<float>(r);
        float phi = ringPos * vertRotation;

        for (uint32_t s = 0; s <= rings; ++s)
        {
            float slicePos = static_cast<float>(s);
            float rotation = slicePos * horzRotation;

            float u = 1.0f - (uInc * slicePos);
            float v = vInc * ringPos;

            meshPX.vertices.push_back({ {
                radius * sin(rotation) * sin(phi),
                radius * cos(phi),
                radius * cos(rotation) * sin(phi)},
                {u, v} });
        }
    }

    CreatePlaneIndices(meshPX.indices, rings, slices);

    return meshPX;
}

MeshPX MeshBuilder::CreateSkySpherePX(uint32_t slices, uint32_t rings, float radius)
{
    MeshPX meshPX;

    const float vertRotation = (MathC::Constants::Pi / static_cast<float>(rings));
    const float horzRotation = (MathC::Constants::TwoPi / static_cast<float>(slices));
    const float uInc = 1.0f / static_cast<float>(slices);
    const float vInc = 1.0f / static_cast<float>(rings);

    for (uint32_t r = 0; r <= rings; ++r)
    {
        float ringPos = static_cast<float>(r);
        float phi = ringPos * vertRotation;

        for (uint32_t s = 0; s <= slices; ++s)
        {
            float slicePos = static_cast<float>(s);
            float rotation = slicePos * horzRotation;

            float u = 1.0f - (uInc * slicePos);
            float v = vInc * ringPos;

            meshPX.vertices.push_back({ {
                radius * cos(rotation) * sin(phi),
                radius * cos(phi),
                radius * sin(rotation) * sin(phi)},
                {u, v} });
        }
    }

    CreatePlaneIndices(meshPX.indices, rings, slices);

    return meshPX;
}

MeshPX MeshBuilder::CreateSkyBoxPX(float size)
{
    MeshPX meshPX;

    const float hs = size * 0.5f;
    const float q = 0.25f;
    const float q2 = 0.50f;
    const float q3 = 0.75f;
    const float t = 1.0 / 3.0f; // 0.335f
    const float t2 = 2.0f * t;

    // Left
    meshPX.vertices.push_back({ { hs, -hs, -hs}, {0.0f, t2} });
    meshPX.vertices.push_back({ { hs,  hs, -hs}, {0.0f, t} });
    meshPX.vertices.push_back({ { hs,  hs,  hs}, {q, t} });
    meshPX.vertices.push_back({ { hs, -hs,  hs}, {q, t2} });

    // Top
    meshPX.vertices.push_back({ { hs,  hs,  hs}, {q, t} });
    meshPX.vertices.push_back({ { hs,  hs, -hs}, {q, 0.0f} });
    meshPX.vertices.push_back({ {-hs,  hs, -hs}, {q2, 0.0f} });
    meshPX.vertices.push_back({ {-hs,  hs,  hs}, {q2, t} });

    // Front
    meshPX.vertices.push_back({ {-hs, -hs,  hs}, {q2, t2} });
    meshPX.vertices.push_back({ {-hs,  hs,  hs}, {q2, t} });
    meshPX.vertices.push_back({ { hs,  hs,  hs}, {q, t} });
    meshPX.vertices.push_back({ { hs, -hs,  hs}, {q, t2} });

    // Bottom
    meshPX.vertices.push_back({ { hs, -hs,  hs}, {q, t2} });
    meshPX.vertices.push_back({ { hs, -hs, -hs}, {q, 1.0f} });
    meshPX.vertices.push_back({ {-hs, -hs, -hs}, {q2, 1.0f} });
    meshPX.vertices.push_back({ {-hs, -hs,  hs}, {q2, t2} });

    // Right
    meshPX.vertices.push_back({ {-hs, -hs, -hs}, {q3, t2} });
    meshPX.vertices.push_back({ {-hs,  hs, -hs}, {q3, t} });
    meshPX.vertices.push_back({ {-hs,  hs,  hs}, {q2, t} });
    meshPX.vertices.push_back({ {-hs, -hs,  hs}, {q2, t2} });

    // Back
    meshPX.vertices.push_back({ {-hs, -hs, -hs}, {q3, t2} });
    meshPX.vertices.push_back({ {-hs,  hs, -hs}, {q3, t} });
    meshPX.vertices.push_back({ { hs,  hs, -hs}, {1.0f, t} });
    meshPX.vertices.push_back({ { hs, -hs, -hs}, {1.0f, t2} });

    meshPX.indices = {
        // Left
        2, 1, 0,
        2, 0, 3,
        // Top
        6, 5, 4,
        6, 4, 7,
        // Front
        8, 9, 10,
        11, 8, 10,
        // Bottom
        12, 13, 14,
        15, 12, 14,
        // Right
        16, 17, 18,
        19, 16, 18,
        // Back
        22, 21, 20,
        22, 20, 23
    };

    return meshPX;
}
