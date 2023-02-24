#include "include/MaterialCommon.hlsli"

struct PS_OUT
{
    float4 p : SV_POSITION;
    float4 n : NORMAL;
    float4 c : COLOR;
    float2 t : TEXCOORD;
};

Texture2D textures[7] : register(t0);

SamplerState samper_state : register(s0);

float4 PS(PS_OUT output) : SV_Target
{
	// Tex	
    float4 base_color = CreateColor(textures[0], samper_state, output.t);
    
	// Light 
    float4 light = -direction * color;
    float bright = max(0.2f, dot(output.n, light));
    float4 normal = { bright, bright, bright, 1 };

    return output.c * base_color * normal;
}