#include "include/LevelHeader.hlsli"

// GS cbuffer
cbuffer edit_option : register(b0)
{
	int altitude;
    int temp0;
    int temp1;  
    int temp2;
}      

[maxvertexcount(3)]
void GS(triangle VS_OUT input[3], inout TriangleStream<GS_IN> stream)
{
	GS_IN gs_stream = (GS_IN)0;  
	  
	float4 edge1 = input[1].p - input[0].p;
	float4 edge2 = input[2].p - input[0].p;
	
	for (int i = 0; i < 3; ++i)
	{
        gs_stream.p = input[i].p;
		gs_stream.n = normalize(cross(edge1, edge2));  
		gs_stream.o = input[i].o;  
        gs_stream.c = input[i].c;
		gs_stream.t = input[i].t;  
        gs_stream.layer_texel = input[i].layer_texel;  
        gs_stream.lod = input[i].lod;
        gs_stream.strength = input[i].strength;

		if (input[i].strength > 0)
		{
			gs_stream.p.y += input[i].strength * altitude;
			gs_stream.o.y += input[i].strength * altitude;
        }
		
        gs_stream.p = mul(gs_stream.p, input[i].view_proj);
		
		stream.Append(gs_stream);
	}
	stream.RestartStrip();
}
