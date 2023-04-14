#include "include/LevelHeader.hlsli"
#include "include/PixelCommon.hlsli"

Texture2D    textures[5]		: register(t0);
Texture2D    alpha_layer        : register(t5);
SamplerState samplers		: register(s0);

float4 PS(GS_IN output) : SV_Target
{         
    float4 base_color = textures[0].SampleLevel(samplers, output.t, output.lod);
    float4 layer_color = alpha_layer.Sample(samplers, output.layer_texel);
    
    float4 tex_color1 = textures[1].SampleLevel(samplers, output.t, output.lod);
    float4 tex_color2 = textures[2].SampleLevel(samplers, output.t, output.lod);
    float4 tex_color3 = textures[3].SampleLevel(samplers, output.t, output.lod);
    float4 tex_color4 = textures[4].SampleLevel(samplers, output.t, output.lod);
    
    base_color = lerp(base_color, tex_color1, layer_color.r);
    base_color = lerp(base_color, tex_color2, layer_color.g);
    base_color = lerp(base_color, tex_color3, layer_color.b);
    base_color = lerp(base_color, tex_color4, layer_color.a);
    
    
    float bright = max(0.2f, dot(output.n, -direction));
    float4 light_color = float4(bright, bright, bright, 1);// * color;

    return base_color * light_color * float4(output.c.xyz, 1);  
}