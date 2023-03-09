struct VS_IN
{
	float3 p : F3_POSITION;
	float3 n : F3_NORMAL;
	float4 c : F4_COLOR;
	float2 t : F2_TEXTURE;  
    float2 layer_texel : F2_TEXEL;
};
  
struct VS_OUT
{
	float4 p : SV_POSITION;
	float3 o : POSITION;  
	float3 n : NORMAL;
	float4 c : COLOR;
    float2 t : TEXCOORD0;
    float2 layer_texel : F2_TEXEL;
	float1 strength : TEXCOORD1;
    matrix view_proj : TEXCOORD2;
    float lod : COLOR1;  
};

struct GS_IN
{
	float4 p : SV_POSITION;  
	float3 o : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
    float2 t : TEXCOORD0;
	float2 layer_texel : TEXCOORD1;	
	
    float strength : TEXCOORD2;
    float lod : COLOR1;
};
