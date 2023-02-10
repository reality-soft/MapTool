struct PS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};


cbuffer cb_light : register(b0)
{
	float4 default_light;
	float light_bright;
}

Texture2D    g_txTex			: register(t0);
SamplerState g_SampleWrap		: register(s0);

float4 PS(PS_OUT input) : SV_Target
{
	// light
	float4 light = default_light * light_bright;

	float4 tex_color = g_txTex.Sample(g_SampleWrap, input.t);
	float light_color = max(0.2f, dot(input.n, -default_light));
	return float4(1, 1, 1, 1);//input.c;// *light_color; // tex_color* light_color;
}