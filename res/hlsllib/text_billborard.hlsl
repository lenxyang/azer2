struct VsInput {
  float3 position: POSITION;
};

struct GsOutput {
  float4 position: SV_POSITION;
  float3 normal:   NORMAL;
  float2 texcoord: TEXCOORD0;
};

cbuffer vs_buffer : register(b0) {
  float4x4 world;
};

cbuffer gs_buffer : register(b0) {
  float4x4 pv;
  float4 viewup;
  float4 viewpos;
  float4 params;
};

cbuffer ps_buffer : register(b0) {
  float4     diffuse;
};
Texture2D diffusemap:  register(t0);
SamplerState texsampler : register(s0) {
  Filter = MIN_MAG_MIP_LINEAR;
};

VsInput vs_main(VsInput input) {
  VsInput o;
  o.position = mul(world, float4(input.position, 1.0));
  return o;
}

[maxvertexcount(30)]
void gs_main(point VsInput v[1], inout TriangleStream<GsOutput> stream) {
  float3 viewz = normalize(v[0].position - viewpos.xyz);
  float3 viewy = normalize(viewup.xyz);
  float3 viewx = normalize(cross(viewy, viewz));
  
  GsOutput o;
  float3 center = v[0].position;
  float3 pos;
  pos = center - params.x * viewx + params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(0.0f, 0.0f);
  stream.Append(o);

  pos = center + params.x * viewx + params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(1.0f, 0.0f);
  stream.Append(o);
  
  pos = center + params.x * viewx - params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(1.0f, 1.0f);
  stream.Append(o);
  stream.RestartStrip();

  pos = center - params.x * viewx + params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(0.0f, 0.0f);
  stream.Append(o);

  pos = center + params.x * viewx - params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(1.0f, 1.0f);
  stream.Append(o);

  pos = center - params.x * viewx - params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(0.0f, 1.0f);
  stream.Append(o);
  stream.RestartStrip();
}

float4 ps_main(GsOutput o):SV_TARGET {
  float4 texcolor = diffusemap.Sample(texsampler, o.texcoord);
  clip(texcolor.a - 0.1f);
  return texcolor * diffuse * texcolor.a;
}
