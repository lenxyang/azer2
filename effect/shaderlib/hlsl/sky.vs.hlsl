
struct VsOutput {
  float4 position : SV_POSITION;
  float2 texcoord : TEXCOORD;
  float4 normal : NORMAL;
  float3 texpos : TEXPOS;
};

struct VSInput {
  float3 position : POSITION;
  float2 texcoord : TEXCOORD;
  float3 normal : NORMAL;
};


cbuffer c_buffer {
   float4x4 pv;
   float4x4 world;
};

VsOutput vs_main(VSInput input) {
  VsOutput o;
  float4 pos = mul(world, float4(input.position, 1.0));
  o.position = mul(pv, pos);
  o.texcoord = input.texcoord;
  o.normal  = mul(world, float4(input.normal, 0.0));
  o.texpos = input.position;
  return o;
}
