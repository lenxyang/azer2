#include "azer/effect/shaderlib/hlsl/light.h.hlsl"

struct VsOutput {
  float4 position:SV_POSITION;
  float3 worldpos: WPOS;
  float3 normal:NORMAL;
  float3 viewin: VIEWIN;
  float2 texcoord: TEXCOORD0;
};

cbuffer c_buffer {
   float      ambient_scalar;
   float      specular_scalar;
   float      alpha;
   float      padding;
   Light      lights[4];
};

Texture2D diffuse_map: register(t0);
SamplerState texsampler;

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
  color += CalcLight(lights[0], o.normal, o.viewin, o.worldpos, mtrl);
  color += CalcLight(lights[1], o.normal, o.viewin, o.worldpos, mtrl);
  return float4(color, mtrl.alpha);
}
