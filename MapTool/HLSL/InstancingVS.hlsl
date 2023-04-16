#include "include/VertexCommon.hlsli"

struct VS_IN
{
    float3 p : F3_POSITION;
    float3 n : F3_NORMAL;    
    float2 t : F2_TEXTURE;
};
struct VS_OUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL0;
    float2 t : TEXCOORD0;
    
    float lod : TEXCOORD1;
    float3 view_dir : TEXCOORD2;
    float3 origin : NORMAL1;
};

struct IntanceData
{
    matrix world_matrix;
};
  
StructuredBuffer<IntanceData> instance_buffer : register(t0);

VS_OUT VS(VS_IN input, uint inst : SV_InstanceID)
{
    VS_OUT output = (VS_OUT) 0;
    
    float4 local = float4(input.p, 1.0f);
    float4 normal = float4(input.n, 1.0f);
    
    float4 world = mul(local, instance_buffer[inst].world_matrix);
    float4 projection = mul(world, ViewProjection());
    
    output.p = projection;
    output.n = normal;
    output.t = input.t;    
        
    output.lod = GetLod(input.p);
    output.view_dir = normalize(camera_world - world).xyz;
    output.origin = world;

    return output;
}