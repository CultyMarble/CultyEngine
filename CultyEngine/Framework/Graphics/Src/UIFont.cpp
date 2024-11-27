#include "Precompiled.h"
#include "UIFont.h"

#include "GraphicsSystem.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
    const wchar_t* GetFontFamily(UIFont::FontType font)
    {
        switch (font)
        {
        case UIFont::FontType::Arial:
            return L"Arial";
            break;
        case UIFont::FontType::Consolas:
            return L"Consolas";
            break;
        case UIFont::FontType::CourierNew:
            return L"Courier New";
            break;
        case UIFont::FontType::TimesNewRoman:
            return L"Times New Roman";
            break;
        case UIFont::FontType::Verdana:
            return L"Verdana";
            break;
        default:
            ASSERT(false, "UIFont: invalid font type");
            break;
        }

        return L"Arial";
    }

    inline uint32_t ToFontColor(const Color& color)
    {
        uint32_t r = (uint32_t)(color.r * 255.0f);
        uint32_t g = (uint32_t)(color.g * 255.0f);
        uint32_t b = (uint32_t)(color.b * 255.0f);
        uint32_t a = (uint32_t)(color.a * 255.0f);

        // r                     1111 1111
        // g           1111 1111 0000 0000
        // b 1111 1111 0000 0000 0000 0000
        return (a << 24) | (b << 16) | (g << 8) | (r);
    }

    std::unique_ptr<UIFont> sUIFont;
}

void UIFont::StaticInitialize(FontType font)
{
    ASSERT(sUIFont == nullptr, "UIFont: is already initialized");
    sUIFont = std::make_unique<UIFont>();
    sUIFont->Initialize(font);
}

void UIFont::StaticTerminate()
{
    if (sUIFont != nullptr)
    {
        sUIFont->Terminate();
        sUIFont.reset();
    }
}

UIFont* UIFont::Get()
{
    ASSERT(sUIFont != nullptr, "UIFont: is not initialized");
    return sUIFont.get();
}

UIFont::~UIFont()
{
    ASSERT(mFontFactory == nullptr && mFontWrapper == nullptr, "UIFont: terminate must be called");
}

void UIFont::Initialize(FontType font)
{
    mFontType = font;
    auto device = GraphicsSystem::Get()->GetDevice();
    FW1CreateFactory(FW1_VERSION, &mFontFactory);
    auto hr = mFontFactory->CreateFontWrapper(device, GetFontFamily(font), &mFontWrapper);
    ASSERT(SUCCEEDED(hr), "UIFont: failed to create font wrapper");
}

void UIFont::Terminate()
{
    SafeRelease(mFontWrapper);
    SafeRelease(mFontFactory);
}

void UIFont::DrawString(const wchar_t* str, const MathC::Vector2& position, float size, const Color& color)
{
    int len = (int)wcslen(str);
    auto context = GraphicsSystem::Get()->GetContext();
    mFontWrapper->DrawString(context, str, size, position.x, position.y,
        ToFontColor(color), FW1_RESTORESTATE);
}

float UIFont::GetStringWidth(const wchar_t* str, float size) const
{
    GraphicsSystem* gs = GraphicsSystem::Get();

    FW1_RECTF layoutRect;
    layoutRect.Left = 0.0f;
    layoutRect.Top = 0.0f;
    layoutRect.Right = (float)gs->GetBackBufferWidth();
    layoutRect.Bottom = (float)gs->GetBackBufferHeight();

    auto rect = mFontWrapper->MeasureString(str,
        GetFontFamily(mFontType), size, &layoutRect, FW1_RESTORESTATE);

    return rect.Right - rect.Left;
}