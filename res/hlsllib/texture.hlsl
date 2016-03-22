#include "azer/res/hlsllib/light.h.hlsl"
#include "azer/res/hlsllib/nmap.h.hlsl"

struct VSInput {
  float3 position:POSITION;
  float3 normal:NORMAL;
  float2 texcoord: TEXCOORD0;
  float3 tang: TANGENT;
};

struct VsOutput {
  float4 position:SV_POSITION;
  float3 worldpos: WPOS;
  float3 normal:NORMAL;
  float3 viewin: VIEWIN;
  float2 texcoord: TEXCOORD0;
  float3 tang: TANGENT;
  float3 binormal : BINORMAL;
};

cbuffer psbuffer {
   float4x4 pv;
   float4x4 world;
   float4   camerapos;
};

cbuffer vsbuffer {
   float      ambient_scalar;
   float      specular_scalar;
   float      alpha;
   int        light_count;
   Light      lights[4];
};

Texture2D diffusemap:  register(t0);
Texture2D specularmap: register(t1);
Texture2D emissionmap: register(t2);
Texture2D alphamap:    register(t3);
Texture2D normalmap:   register(t4);
SamplerState texsampler;

VsOutput vs_main0(VSInput input) {
  VsOutput o;
  float4 pos = float4(input.position, 1.0f);
  float4 normal = float4(input.normal, 0.0f);
  float4 worldpos = mul(world, pos);
  o.position = mul(pv, worldpos);
  o.worldpos = worldpos.xyz;
  o.normal = normalize(mul(world, normal)).xyz;
  o.viewin = normalize(camerapos.xyz - o.worldpos);
  o.texcoord = input.texcoord;
  return o;
}

VsOutput vs_main1(VSInput input) {
  VsOutput o;
  float4 pos = float4(input.position, 1.0f);
  float4 normal = float4(input.normal, 0.0f);
  float4 worldpos = mul(world, pos);
  o.position = mul(pv, worldpos);
  o.worldpos = worldpos.xyz;
  o.normal = normalize(mul(world, normal)).xyz;
  o.tang = normalize(mul(world, float4(input.tang, 0.0))).xyz;
  o.binormal = normalize(cross(o.tang, o.normal));
  o.viewin = normalize(camerapos - o.worldpos);
  o.texcoord = input.texcoord;
  return o;
}

float4 ps_main0(VsOutput o):SV_TARGET {
  float4 texcolor = diffusemap.Sample(texsampler, o.texcoord);
  float4 specolor = specularmap.Sample(texsampler, o.texcoord);
  float4 emicolor = emissionmap.Sample(texsampler, o.texcoord);
  float3 normal = o.normal;
  Material mtrl;
  mtrl.ambient  = texcolor * ambient_scalar;
  mtrl.diffuse  = texcolor * specular_scalar;
  mtrl.specular = specolor;
  mtrl.emission = emicolor;
  mtrl.power    = 4;
  mtrl.alpha    = alpha;
  float3 color = float3(0, 0, 0);
  for (int i = 0; i < light_count; ++i) {
    color += CalcLight(lights[i], o.normal, o.viewin, o.worldpos, mtrl);
  }
  return float4(color, mtrl.alpha);
}

float4 ps_main1(VsOutput o):SV_TARGET {
  float4 texcolor = diffusemap.Sample(texsampler, o.texcoord);
  float4 specolor = specularmap.Sample(texsampler, o.texcoord);
  float4 emicolor = emissionmap.Sample(texsampler, o.texcoord);
  float3 normal =   normalmap.Sample(texsampler, o.texcoord).xyz;
  normal = TBNToWorldSpace(normal.xyz, o.tang, o.normal, o.binormal);
  Material mtrl;
  mtrl.ambient  = texcolor * ambient_scalar;
  mtrl.diffuse  = texcolor * specular_scalar;
  mtrl.specular = specolor;
  mtrl.emission = emicolor;
  mtrl.power    = 4;
  mtrl.alpha    = alpha;
  float3 color = float3(0, 0, 0);
  for (int i = 0; i < light_count; ++i) {
    color += CalcLight(lights[i], normal, o.viewin, o.worldpos, mtrl);
  }
  return float4(color, mtrl.alpha);
}

