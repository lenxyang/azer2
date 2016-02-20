#pragma pack_matrix(row_major)

struct VsOutput {
  float4 position : SV_POSITION;
  float4 npos     : NORMAL;
};

struct VSInput {
  float3 position:POSITION;
  float3 normal:NORMAL;
  float2 texcoord: TEXCOORD0;
};

cbuffer c_buffer {
   float4x4 pv;
   float4x4 world;
   float linelength;
};

VsOutput vs_main(VSInput input) {
  VsOutput o;
  float4 worldpos = mul(world, float4(input.position, 1.0));
  float4 normal = mul(world, float4(input.normal, 0.0));
  o.npos = worldpos + float4(normalize(normal.xyz) * 0.1, 0.0);
  o.position = mul(pv, worldpos);
  o.npos = mul(pv, o.npos);
  return o;
}
