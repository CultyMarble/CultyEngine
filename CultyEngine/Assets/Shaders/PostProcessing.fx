// This does Post Processing Effect
cbuffer PostProcessingBuffer : register(b0)
{
    int mode;
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
    float4 finalColor = 1.0f;
    
    if (mode == 0)      // None
    {
        finalColor = textureMap0.Sample(textureSampler, input.texCoord);
    }
    else if (mode == 1) // Monochrome
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = (color.r + color.r + color.r) / 3.0f;
    }
    else if (mode == 2) // Invert
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = 1.0f - color;
    }
    else if (mode == 3) // Mirror
    {
        float2 texCoord = input.texCoord;
        texCoord.x *= params0;
        texCoord.y *= params1;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    else if (mode == 4) // Blur
    {
        float u = input.texCoord.x;
        float v = input.texCoord.y;
        finalColor =
        textureMap0.Sample(textureSampler, float2(u, v))
        + textureMap0.Sample(textureSampler, float2(u + params0, v))
        + textureMap0.Sample(textureSampler, float2(u - params0, v))
        + textureMap0.Sample(textureSampler, float2(u, v + params1))
        + textureMap0.Sample(textureSampler, float2(u, v - params1))
        + textureMap0.Sample(textureSampler, float2(u + params0, v - params1))
        + textureMap0.Sample(textureSampler, float2(u + params1, v + params1))
        + textureMap0.Sample(textureSampler, float2(u - params1, v - params1))
        + textureMap0.Sample(textureSampler, float2(u - params1, v + params1));
        
        finalColor *= 0.1111f;
    }
    else if (mode == 5) // Combine2
    {
        float color0 = textureMap0.Sample(textureSampler, input.texCoord);
        float color1 = textureMap1.Sample(textureSampler, input.texCoord);
        finalColor = (color0 + color1) * 0.5f;
        // finalColor = ((1.0f - color1.a) * color0) + (color1.a * color1);
    }
    else if (mode == 6) // Chromatic Abberation
    {
        float2 distortion = float2(params0, -params1);
        float4 R_Channel = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
        float4 G_Channel = textureMap0.Sample(textureSampler, input.texCoord);
        float4 B_Channel = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
        finalColor = float4(R_Channel.r, G_Channel.g, B_Channel.b, 1.0f);
    }
    else if (mode == 7)
    {
        float waveValue = input.texCoord.x * (3.14159f * params0);
        float2 texCoord = input.texCoord;
        texCoord.y += sin(waveValue) * params1;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    
    return finalColor;
}