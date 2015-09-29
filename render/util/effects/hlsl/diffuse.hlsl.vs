#pragma pack_matrix(row_major)

struct VsOutput {
  float4 position:SV_POSITION;
  float4 normal:NORMAL;
  float2 texcoord0: TEXCOORD0;
  float2 texcoord1: TEXCOORD1;
};

struct VSInput {
  float4 position:POSITION;
  float4 normal:NORMAL;
  float2 texcoord0: TEXCOORD0;
  float2 texcoord1: TEXCOORD1;
};

cbuffer c_buffer {
   float4x4 pvw;
   float4x4 world;
};

VsOutput vs_main(VSInput input) {
  VsOutput o;
  o.position = mul(pvw, input.position);
  o.normal = mul(world, input.normal);
  o.texcoord0 = input.texcoord0;
  o.texcoord1 = input.texcoord1;
  return o;
}
