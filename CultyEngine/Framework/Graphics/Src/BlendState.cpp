#include "Precompiled.h"
#include "BlendState.h"

#include "GraphicsSystem.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
	D3D11_BLEND GetSrcBlend(BlendState::BlendMode mode)
	{
		switch (mode)
		{
		case BlendState::BlendMode::Additive:
			return D3D11_BLEND_SRC_ALPHA;
		case BlendState::BlendMode::AlphaBlend:
			return D3D11_BLEND_SRC_ALPHA;
		case BlendState::BlendMode::AlphaPremultiplied:
			return D3D11_BLEND_ONE;
		case BlendState::BlendMode::Opaque:
			return D3D11_BLEND_ONE;
		default:
			ASSERT(false, "BlendState: Unmapped BlendMode!");
			break;
		}

		return D3D11_BLEND_ONE;
	}

	D3D11_BLEND GetDestBlend(BlendState::BlendMode mode)
	{
		switch (mode)
		{
		case BlendState::BlendMode::Additive:
			return D3D11_BLEND_ONE;
		case BlendState::BlendMode::AlphaBlend:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendState::BlendMode::AlphaPremultiplied:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendState::BlendMode::Opaque:
			return D3D11_BLEND_ZERO;
		default:
			ASSERT(false, "BlendState: Unmapped BlendMode!");
			break;
		}

		return D3D11_BLEND_ZERO;
	}
}

void BlendState::ClearState()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(nullptr, nullptr, UINT_MAX);
}

BlendState::~BlendState()
{
	ASSERT(mBlendState == nullptr, "BlendState: Terminate must be called!");
}

void BlendState::Initialize(BlendMode mode)
{
	D3D11_BLEND srcBlend = GetSrcBlend(mode);	//
	D3D11_BLEND destBlend = GetDestBlend(mode);	//

	D3D11_BLEND_DESC desc = {};
	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBlendState(&desc, &mBlendState);
	ASSERT(SUCCEEDED(hr), "BlendState: Failed to initialize");
}

void BlendState::Terminate()
{
	SafeRelease(mBlendState);
}

void BlendState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
}