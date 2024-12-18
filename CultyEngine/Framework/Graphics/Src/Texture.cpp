#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include "DirectXTK/Inc/WICTextureLoader.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

void Texture::UnbindPS(uint32_t slot)
{
    static ID3D11ShaderResourceView* dummy = nullptr;
    GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

Texture::~Texture()
{
    ASSERT(mShaderResourceView == nullptr, "Texture: Must call terminate");
}

Texture::Texture(Texture&& rhs) noexcept
    :mShaderResourceView(rhs.mShaderResourceView)
{
    rhs.mShaderResourceView = nullptr;
}

Texture& Texture::operator=(Texture&& rhs) noexcept
{
    mShaderResourceView = rhs.mShaderResourceView;
    rhs.mShaderResourceView = nullptr;

    return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
    auto device = GraphicsSystem::Get()->GetDevice();
    auto context = GraphicsSystem::Get()->GetContext();

    HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);
    ASSERT(SUCCEEDED(hr), "Texture: Failed to create texture %ls", fileName.c_str());

    ID3D11Resource* resource = nullptr;
    mShaderResourceView->GetResource(&resource);

    ID3D11Texture2D* texture2D = nullptr;
    hr = resource->QueryInterface(&texture2D);
    ASSERT(SUCCEEDED(hr), "Texture: failed to find texture data");

    D3D11_TEXTURE2D_DESC desc;
    texture2D->GetDesc(&desc);
    mWidth = static_cast<uint32_t>(desc.Width);
    mHeight = static_cast<uint32_t>(desc.Height);

    SafeRelease(texture2D);
    SafeRelease(resource);
}

void Texture::Initialize(uint32_t width, uint32_t heigth, Format format)
{
    ASSERT(false, "Texture: Not yet implemented!");
}

void Texture::Initialize(uint32_t width, uint32_t height, Format format, const void* data)
{
    auto device = GraphicsSystem::Get()->GetDevice();
    ASSERT(device != nullptr, "Texture: Device pointer is null!");
    ASSERT(data != nullptr, "Texture: Pixel data is null!");

    // Describe the texture
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = GetDXGIFormat(format);
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // Validate pixel data size
    size_t expectedSize = width * height * 4; // Assuming RGBA8
    LOG("Expected Pixel Data Size: %zu", expectedSize);

    // Initialize subresource data
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = data;
    initData.SysMemPitch = width * 4; // Correct row pitch
    LOG("SysMemPitch: %u, Width: %u, Height: %u, Format: %d", initData.SysMemPitch, width, height, desc.Format);

    ASSERT(data != nullptr, "Pixel data is null!");
    ASSERT(reinterpret_cast<uintptr_t>(data) % 4 == 0, "Pixel data not 4-byte aligned!");

    const uint8_t* byteData = static_cast<const uint8_t*>(data);
    LOG("Pixel Data Check: First Byte = %u, Last Byte = %u", byteData[0], byteData[expectedSize - 1]);
    LOG("Width = %u, Height = %u, SysMemPitch = %u", width, height, initData.SysMemPitch);

    // Create the texture
    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);
    if (FAILED(hr))
    {
        LOG("Failed to create texture. HRESULT: 0x%08X", hr);
        ASSERT(false, "Texture: Failed to create texture from raw data!");
    }

    // Create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(texture, &srvDesc, &mShaderResourceView);
    if (FAILED(hr))
    {
        LOG("Failed to create shader resource view. HRESULT: 0x%08X", hr);
        SafeRelease(texture);
        ASSERT(false, "Texture: Failed to create shader resource view!");
    }

    mWidth = width;
    mHeight = height;

    SafeRelease(texture);
}

void Texture::Terminate()
{
    SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void* Texture::GetRawData() const
{
    return mShaderResourceView;
}

uint32_t Texture::GetWidth() const
{
    return mWidth;
}

uint32_t Texture::GetHeight() const
{
    return mHeight;
}

DXGI_FORMAT Texture::GetDXGIFormat(Format format)
{
    switch (format)
    {
    case Format::RGBA_U8:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    case Format::RGBA_U32:
        return DXGI_FORMAT_R32G32B32A32_UINT;
        break;
    default:
        break;
    }

    return DXGI_FORMAT_R8G8B8A8_UNORM;
}