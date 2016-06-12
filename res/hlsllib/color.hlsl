#include "azer/res/hlsllib/light.h.hlsl"

struct VsOutput {
  float4 position:SV_POSITION;
  float3 worldpos: WPOS;
  float3 normal:NORMAL;
  float3 viewin: VIEWIN;
  float2 texcoord: TEXCOORD0;
};

struct VSInput {
  float3 position:POSITION;
  float3 normal:NORMAL;
  float2 texcoord: TEXCOORD0;
};

cbuffer vs_buffer : register(b0) {
   float4x4 pv;
   float4x4 world;
   float4   camerapos;
};

cbuffer ps_buffer : register(b0) {
   float4     diffuse;
   float4     ambient;
   float4     specular;
   float4     emission;
   float      specular_power;
   float      alpha;
   float2     padding;
   Light      lights[4];
   int        light_count;
};

VsOutput vs_main(VSInput input) {
  VsOutput o;
  float4 pos = float4(input.position, 1.0f);
  float4 normal = float4(input.normal, 0.0f);
  o.worldpos = mul(world, pos).xyz;
  o.position = mul(pv, float4(o.worldpos, 1.0));
  o.normal = normalize(mul(world, normal)).xyz;
  o.viewin = normalize(camerapos - o.worldpos);
  o.texcoord = input.texcoord;
  return o;
}

float4 ps_main(VsOutput o):SV_TARGET {
  float3 normal = o.normal;
  Material mtrl;
  mtrl.ambient  = ambient;
  mtrl.specular = specular;
  mtrl.diffuse  = diffuse;
  mtrl.emission = emission;
  mtrl.power    = specular_power;
  mtrl.alpha    = alpha;
  float3 color = float3(0, 0, 0);
  for (int i = 0; i < light_count; ++i) {
    color += CalcLight(lights[i], o.normal, o.viewin, o.worldpos, mtrl);
  }
  return float4(color, mtrl.alpha);
}
