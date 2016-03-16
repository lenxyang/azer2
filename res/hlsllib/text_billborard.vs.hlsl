
struct VsInput {
  float3 position: POSITION;
  float3 normal:   NORMAL;
  float2 texcoord: TEXCOORD0;
};

cbuffer c_buffer {
  float4x4 world;
};

VsInput vs_main(VsInput input) {
  VsInput o;
  o.position = mul(world, float4(input.position, 1.0));
  o.normal = mul(world, float4(input.normal, 0.0f)).xyz;
  o.texcoord = input.texcoord;
  return o;
}
