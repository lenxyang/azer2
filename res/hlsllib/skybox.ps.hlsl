
TextureCube cube : register(t0);
SamplerState texsampler;

struct VsOutput {
  float4 position : SV_POSITION;
  float2 texcoord : TEXCOORD;
  float4 normal : NORMAL;
  float3 texpos : TEXPOS;
};

float4 ps_main(VsOutput v) : SV_TARGET {
  float4 color = cube.Sample(texsampler, v.texpos);
  return color;
}
