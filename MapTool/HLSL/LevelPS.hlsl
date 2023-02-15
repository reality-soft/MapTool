#include "LevelHeader.hlsli"


cbuffer cb_light : register(b0)
{
	float4 default_light;
	float light_bright;
}

Texture2D    g_txTex			: register(t0);
SamplerState g_SampleWrap		: register(s0);

float4 PS(VS_OUT input) : SV_Target
{
	// Tex
	float4 tex_color = g_txTex.Sample(g_SampleWrap, input.t);

	// Light
	float4 light = default_light * light_bright;
	float bright = max(0.2f, dot(input.n, -default_light));

	// Stringth
	float4 selected_color = { (input.strength * 3) + 1, 1.0f, 1.0f, 1.0f };

	return input.c * tex_color * bright * selected_color;
}