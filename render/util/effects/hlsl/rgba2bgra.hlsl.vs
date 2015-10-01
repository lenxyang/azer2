#pragma pack_matrix(row_major)

struct VsOutput {
  float4 position:SV_POSITION;
  float2 texcoord: TEXCOORD;
};

struct VSInput {
  float4 position:POSITION;
  float2 texcoord: TEXCOORD;
};

VsOutput vs_main(VSInput input) {
  VsOutput o;
  o.position = input.position;
  o.texcoord = input.texcoord;
  return o;
}
