#include "include/LevelHeader.hlsli"
#include "include/VertexCommon.hlsli"

cbuffer cb_hitcircle : register(b1)
{
	bool   is_hit;
	float  circle_radius;
	float4 hitpoint;
	float4 circle_color;
}

VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	float4 local = float4(input.p, 1.0f);
    float4 edit_circle = { 1, 1, 1, 1 };
	if (is_hit)
	{
        float length_from_point = length((float4(input.p, 1.0f) - hitpoint));
		if (length_from_point <= circle_radius)
		{
			output.strength =  1.0f - length_from_point / circle_radius;
            edit_circle = float4((output.strength * 3) + 1, 1.0f, 1.0f, 1.0f);
        }
		else
		{
			output.strength = 0.0f;
            edit_circle = float4(1.0f, 1.0f, 1.0f, 1.0f);
        }
	}
	
    output.lod = GetLod(input.p);
    output.view_proj = ViewProjection();
    output.p = local;
	output.o = input.p;
	output.n = input.n;
    output.c = edit_circle;
	output.t = input.t;
    output.layer_texel = input.layer_texel;

	return output;
}