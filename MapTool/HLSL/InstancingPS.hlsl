#include "include/PixelCommon.hlsli"

struct PS_OUT
{
    float4 p : SV_POSITION;
    float4 n : NORMAL;
    float2 t : TEXCOORD;
    float lod : TEXCOORD1;
};

Texture2D textures[7] : register(t0);
SamplerState samper_state : register(s0);

float4 PS(PS_OUT input) : SV_Target
{
    float4 albedo = textures[0].Sample(samper_state, input.t);    
    return albedo;
    
    //return float4(input.n, 1);
} 