// This does Post Processing Effect
cbuffer PostProcessingBuffer : register(b0)
{
    float params0;
    float params1;
    float params2;
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
float4 PS(VS_INPUT input) : SV_Target
{    
    float2 uv = input.texCoord;
    
    // Chromatic Aberration
    float2 distortion = float2(params0, -params0);
    float4 R_Channel = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
    float4 G_Channel = textureMap0.Sample(textureSampler, input.texCoord);
    float4 B_Channel = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord); 
    float4 finalColor = float4(R_Channel.r, G_Channel.g, B_Channel.b, 1.0f);
    
    // Scan Line
    float scanline = 0.75f + 0.05f * sin((uv.y + params2) * params1);
    finalColor *= scanline;
    
    return finalColor;
}