// This does Post Processing Effect
cbuffer PostProcessingBuffer : register(b0)
{    
    float edgeDistortion;
    float abberationSeperation;
    float scanlineFrequency;
    float scanlineBrightness;
    float scanlineAlpha;
    float scanlineOffsetY;
    float bloomThreshold;
    float bloomStrength;
    float noiseStrength;
    float noiseAlpha;
    float noiseRandom;
    float jitterThickness;
    float jitterStrength;
    float jitterRandom;
    float breakFrequency;
    float breakStrength;
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
    float4 finalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // Edges Curvature
    float2 center = float2(0.5f, 0.5f);
    float aspectRatio = 16.0f / 9.0f;

    // Scale UVs to maintain aspect ratio and account for distortion
    float2 scaledUV = (uv - center) * float2(aspectRatio, 1.0) + center;

    float2 uvDistort = scaledUV - center;
    float yDistort = scaledUV.y - center.y;
    
    uvDistort *= 1.0 + edgeDistortion * (uvDistort.x * uvDistort.x + uvDistort.y * uvDistort.y);
    uv = uvDistort + center;
    
    // Scale back to original UVs
    uv = (uv - center) / float2(aspectRatio, 1.0) + center;

    // Check for UV wrapping
    bool isWrapped = (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0);
    
    // Scan Line
    float scanline = sin((uv.y + scanlineOffsetY) * scanlineFrequency) * scanlineBrightness + scanlineAlpha;
    finalColor *= scanline;
    
    // Jitter Effect
    if (frac(sin((uv.y + scanlineOffsetY) * scanlineFrequency)) < jitterThickness)
    {
        float randomOffset = frac(sin(uv.y * jitterRandom) * 43758.5453f) * jitterStrength;
        uv.x += randomOffset;
    }
    
    // Break Effect
    uv.x += floor(sin(uv.y + breakFrequency)) * breakStrength;
    uv.x -= floor(sin(uv.y + breakFrequency + 0.01f)) * breakStrength;
    
    // Chromatic Aberration
    float3 Channel_R = textureMap0.Sample(textureSampler, uv + float2(abberationSeperation, 0));
    float3 Channel_G = textureMap0.Sample(textureSampler, uv);
    float3 Channel_B = textureMap0.Sample(textureSampler, uv - float2(abberationSeperation, 0));
    finalColor *= float4(Channel_R.r, Channel_G.g, Channel_B.b, 1.0f);
    
    // Simple Bloom
    float4 bloom = (finalColor - bloomThreshold) * bloomStrength;
    bloom = saturate(bloom);
    finalColor += bloom;
    
    // Noise
    float noise = (rand(uv * noiseRandom) - noiseStrength) * noiseAlpha;
    finalColor += noise;
    finalColor = saturate(finalColor);
    
    // Set color to black if UVs are wrapped
    if (isWrapped)
        finalColor *= float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return finalColor;
}