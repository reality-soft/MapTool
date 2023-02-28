#include "include/PixelCommon.hlsli"

struct PS_OUT
{
    float4 p : SV_POSITION;  
    float4 n : NORMAL;
    float4 c : COLOR;
    float2 t : TEXCOORD;
    float lod : TEXCOORD1;
};

Texture2D textures[7] : register(t0);
SamplerState samper_state : register(s0);

float4 PS(PS_OUT output) : SV_Target
{  
    // VertexColor  
    float4 vertex_color = output.c;
    
	// Tex	
    float4 base_color = textures[0].SampleLevel(samper_state, output.t, output.lod);
      
	// Light 
    float bright = max(0.2f, dot(output.n, -direction));
    float4 light_color = float4(bright, bright, bright, 1) * color;

    return output.c * base_color * light_color;

}