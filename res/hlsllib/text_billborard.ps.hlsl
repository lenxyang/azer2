struct GsOutput {
  float4 position: SV_POSITION;
  float3 normal:   NORMAL;
  float2 texcoord: TEXCOORD0;
};

cbuffer c_buffer {
  float4     diffuse;
};
Texture2D diffusemap:  register(t0);
SamplerState texsampler;

float4 ps_main(GsOutput o):SV_TARGET {
  float4 texcolor = diffusemap.Sample(texsampler, o.texcoord);
  clip(texcolor.a - 0.1f);
  return texcolor * diffuse;
}
