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
	srand(time(nullptr));
	int index = rand() & 100;

	MeshPC mesh;

	const float hs = size * 0.5f;
	
	// front
	mesh.vertices.push_back({ {-hs, -hs, -hs}, GetNextColor(index)});
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
				GetNextColor(index)});
		}
	}

	CreatePlaneIndices(meshPC.indices, rings, slices);

	return meshPC;
}
