// simple shader that only uses position and texCoord and applied transform

cbuffer ConstantBuffer : register(b0)
{
    matrix wvp; // world view projection
};

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 textCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.textCoord = input.textCoord;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return textureMap.Sample(textureSampler, input.textCoord);
}