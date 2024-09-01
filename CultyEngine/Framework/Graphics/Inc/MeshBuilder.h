#pragma once
#include "MeshTypes.h"

namespace CultyEngine::Graphics
{
    class MeshBuilder
    {
    public:
        // Pyramid
        static MeshPC CreatePyramidPC(float size);

        // Cude
        static MeshPC CreateCubePC(float size);

        // Rectangle
        static MeshPC CreateRectanglePC(float width, float height, float depth);

        // Vertical Plane
        static MeshPC CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);
        static MeshPX CreateVerticalPlanePX(uint32_t numRows, uint32_t numCols, float spacing);

        // Horizontal Plane
        static MeshPC CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);
        static MeshPX CreateHorizontalPlanePX(uint32_t numRows, uint32_t numCols, float spacing);
        static Mesh CreateHorizontalPlane(uint32_t numRows, uint32_t numCols, float spacing);

        // Create Cylinder
        static MeshPC CreateCylinderPC(uint32_t slice, uint32_t rings);

        // Create Sphere
        static MeshPC CreateSpherePC(uint32_t slices, uint32_t rings, float radius);
        static MeshPX CreateSpherePX(uint32_t slices, uint32_t rings, float radius);
        static Mesh CreateSphere(uint32_t slices, uint32_t rings, float radius);

        // Sky Stuff
        static MeshPX CreateSkySpherePX(uint32_t slices, uint32_t rings, float radius);
        static MeshPX CreateSkyBoxPX(float size);

        // Screen Stuff
        static MeshPX CreateScreenQuad();

        // Sprite Quad
        static Mesh CreateSpriteQuad(float width, float height);
    };
}