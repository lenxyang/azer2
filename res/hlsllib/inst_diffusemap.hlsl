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
  float4 world0: WORLD0;
  float4 world1: WORLD1;
  float4 world2: WORLD2;
  float4 world3: WORLD3;
//  float4x4 world: WORLD;
};

cbuffer vs_buffer : register(cb0) {
   float4x4 pv;
   float4x4 pworld;
   float4   camerapos;
};

Texture2D diffuse_map: register(t0);
SamplerState texsampler;

VsOutput vs_main(VSInput v) {
  VsOutput o;
  float4x4 world = float4x4(v.world0, v.world1, v.world2, v.world3);
      
  o.worldpos = mul(world, float4(v.position, 1.0)).xyz;
  o.position = mul(pv, float4(o.worldpos, 1.0));
  o.normal = normalize(mul(world, float4(v.normal, 0.0)).xyz);
  o.viewin = normalize(camerapos - o.worldpos);
  o.texcoord = v.texcoord;
  return o;
}

float4 ps_main(VsOutput o):SV_TARGET {
  float4 texcolor = diffuse_map.Sample(texsampler, o.texcoord);
  float3 normal = o.normal;
  Material mtrl;
  mtrl.ambient  = (ambient_scalar) * texcolor;
  mtrl.specular = specular_scalar * texcolor;
  mtrl.diffuse  = texcolor;
  mtrl.emission = float4(0.0f, 0.0f, 0.0f, 0.0f);
  mtrl.power    = 4;
  mtrl.alpha    = alpha;
  float3 color = float3(0, 0, 0);
  for (int i = 0; i < light_count; ++i) {
    color += CalcLight(lights[i], o.normal, o.viewin, o.worldpos, mtrl);
  }
  return float4(color, mtrl.alpha);
}
