#include "include/LevelHeader.hlsli"
#include "include/MeshCommon.hlsli"

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
	
	if (is_hit)
	{
		float length_from_point = length((input.p - hitpoint));
		if (length_from_point <= circle_radius)
		{
			output.strength =  1.0f - length_from_point / circle_radius;
			output.circle = float4((output.strength * 3) + 1, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			output.strength = 0.0f;
			output.circle = float4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	float4 vLocal = float4(input.p, 1.0f);
    output.mat_viewproj = mul(view_matrix, projection_matrix);
	
    output.p = vLocal;
	output.o = input.p;
	output.n = input.n;
	output.c = input.c * output.circle;
	output.t = input.t;

	return output;
}