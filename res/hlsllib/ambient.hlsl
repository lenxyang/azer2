
struct VSInput {
  float3 position:POSITION;
  float3 normal:NORMAL;
  float2 texcoord: TEXCOORD0;
};

struct VsOutput {
  float4 position:SV_POSITION;
  float3 worldpos: WPOS;
  float3 normal:NORMAL;
  float3 viewin: VIEWIN;
  float2 texcoord: TEXCOORD0;
};

cbuffer vs_buffer : register(b0) {
   float4x4 pv;
   float4x4 world;
   float4   camerapos;
};

cbuffer ps_buffer : register(b0) {
   float4     ambient;
};

VsOutput vs_main(VSInput input) {
  VsOutput o;
  float4 pos = float4(input.position, 1.0f);
  float4 normal = float4(input.normal, 0.0f);
  o.worldpos = mul(world, pos).xyz;
  o.position = mul(pv, float4(o.worldpos, 1.0));
  o.normal = normalize(mul(world, normal).xyz);
  o.texcoord = input.texcoord;
  o.viewin = normalize(camerapos.xyz - o.worldpos.xyz);
  return o;
}

float4 ps_main(VsOutput o):SV_TARGET {
  return ambient;
}
