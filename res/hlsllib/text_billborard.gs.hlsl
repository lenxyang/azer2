
struct VsInput {
  float4 position: POSITION;
  float3 normal:   NORMAL;
  float2 texcoord: TEXCOORD0;
};

VsInput vs_main(VSInput input) {
  return input;
}
