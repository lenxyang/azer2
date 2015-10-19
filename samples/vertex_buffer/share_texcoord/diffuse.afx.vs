// using row_major
#pragma pack_matrix(row_major)

struct VsOutput {
  float4 position:SV_POSITION;
  float2 texcoord:TEXCOORD;
};
struct VSInput {
  float4 position:POSITION;
  float2 texcoord:TEXCOORD;
};
cbuffer c_buffer {
   float4x4 pvw;
};
VsOutput vs_main(VSInput input) {
  VsOutput o;
  o.position = mul(pvw, input.position);
  o.texcoord = input.texcoord;
  return o;
}
