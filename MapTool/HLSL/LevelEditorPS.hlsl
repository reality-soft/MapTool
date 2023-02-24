#include "include/LevelHeader.hlsli"
#include "include/MaterialCommon.hlsli"

Texture2D    g_txTex			: register(t0);
SamplerState g_SampleWrap		: register(s0);

float4 PS(GS_IN output) : SV_Target
{
	// Tex
	float4 tex_color = g_txTex.SampleLevel(g_SampleWrap, output.t, -10);

	// Light
    float4 light = -direction * color;
    float bright = max(0.2f, dot(output.n, light));
	float4 normalmap = { bright , bright , bright , 1};

	return output.c * tex_color * normalmap;
}