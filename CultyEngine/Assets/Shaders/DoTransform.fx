// simple shader that only uses position and color and applied transform

cbuffer ConstantBuffer : register(b0)
{
    matrix wvp; // world view projection
};

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.color = input.color;
    
    return output;
}

float4 PS(VS_OUTPUT output) : SV_Target
{
    return output.color;
}