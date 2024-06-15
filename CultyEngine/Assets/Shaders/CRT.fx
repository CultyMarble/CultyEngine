// This does Post Processing Effect
cbuffer PostProcessingBuffer : register(b0)
{
    float params0;
    float params1;
    float params2;
    float params3;
    float params4;
    float params5;
    float params6;
    float params7;
    float params8;
    float params9;
    float params10;
    float params11;
    float params12;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453f);
}

float4 PS(VS_INPUT input) : SV_Target
{    
    float2 uv = input.texCoord;

    // Apply vertical curvature for CRT effect (only top and bottom)
    float2 center = float2(0.5, 0.5);
    float distortionX = 0.4;
    float distortionY = 0.2;
    float xDistort = uv.x - center.x;
    float yDistort = uv.y - center.y;
    uv.x += distortionX * (xDistort * xDistort - 0.25);
    uv.y += distortionY * (yDistort * yDistort - 0.25);

    // Ensure UV coordinates are within valid range
    uv = clamp(uv, 0.0, 1.0);
    
    // Horizontal Distortion
    //uv.x += floor(sin(uv.y + params10)) * params11;
    if (frac(sin((uv.y + params4) * params1 )) < params10)
    {
        float randomOffset = frac(sin(uv.y * params12) * 43758.5453f) * params11;
        uv.x += randomOffset;
    }
    
    // Chromatic Aberration
    float3 Channel_R = textureMap0.Sample(textureSampler, uv + float2(params0, 0));
    float3 Channel_G = textureMap0.Sample(textureSampler, uv);
    float3 Channel_B = textureMap0.Sample(textureSampler, uv - float2(params0, 0));
    float4 finalColor = float4(Channel_R.r, Channel_G.g, Channel_B.b, 1.0f);
    
    // Scan Line
    float scanline = sin((uv.y + params4) * params1) * params2 + params3;
    finalColor *= scanline;
    
    // Simple Bloom
    float4 bloom = (finalColor - params5) * params6;
    bloom = saturate(bloom);
    finalColor += bloom;
    
    // Noise (additive)
    float noise = (rand(uv * params9) - params7) * params8;
    finalColor += noise;
    finalColor = saturate(finalColor);
    
    return finalColor;
}