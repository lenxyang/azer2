#include "azer/res/hlsllib/light.h.hlsl"
#include "azer/res/hlsllib/nmap.h.hlsl"

struct VsOutput {
  float4 position:SV_POSITION;
  float3 worldpos: WPOS;
  float3 normal:NORMAL;
  float3 viewin: VIEWIN;
  float2 texcoord: TEXCOORD0;
  float3 tang: TANGENT;
  float3 binormal: BINORMAL;
};


cbuffer c_buffer {
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


float4 ps_main(VsOutput o):SV_TARGET {
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

