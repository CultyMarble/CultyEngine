#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include "DirectXTK/Inc/WICTextureLoader.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "Texture: Must call terminate");
}

Texture::Texture(Texture&&) noexcept
{

}

void Texture::Initialize(const std::filesystem::path& fileName)
{

}

void Texture::Initialize(uint32_t width, uint32_t heigth, Format format)
{

}

void Texture::Terminate()
{

}

void Texture::BindVS(uint32_t slot) const
{

}

void Texture::BindPS(uint32_t slot) const
{

}

void* Texture::GetRawData() const
{
	return nullptr;
}

DXGI_FORMAT Texture::GetDXGIFormat(Format format)
{
	return DXGI_FORMAT();
}