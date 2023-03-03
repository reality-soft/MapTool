#include "include/LevelHeader.hlsli"
#include "include/PixelCommon.hlsli"


Texture2D    textures[16]		: register(t0);
SamplerState samplers		: register(s0);

float4 PS(GS_IN output) : SV_Target
{    
    float4 tex_layer[16];
    for (int i = 0; i < 16; ++i)
    {
        float4 color = textures[i].SampleLevel(samplers, output.t, output.lod);
        
        if (length(color) <= 0)
            tex_layer[i] = float4(1, 1, 1, 1);
        else
            tex_layer[i] = color;
    }
    
    int layer_index = (int) output.c.w;
    float4 base_color = tex_layer[layer_index];
    
    if ((int) output.c.w > 0)
    {
        float strength = output.c.w - (float) layer_index;

        if (strength > 0)
        {
            float4 other_layer1 = tex_layer[(int) output.c.w - 1];
        
            base_color = lerp(base_color, other_layer1, 1.0f - strength);
        }
    }

    
    float bright = max(0.2f, dot(output.n, -direction));
    float4 light_color = float4(bright, bright, bright, 1) * color;  

    return base_color * light_color * float4(output.c.xyz, 1);
}