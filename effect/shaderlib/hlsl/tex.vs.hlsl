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

cbuffer c_buffer {
   float4x4 pv;
   float4x4 world;
   float4   camerapos;
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
