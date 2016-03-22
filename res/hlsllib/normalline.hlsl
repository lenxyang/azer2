
struct VsOutput {
  float4 position : SV_POSITION;
  float4 npos     : NORMAL;
};

struct VSInput {
  float3 position:POSITION;
  float3 normal:NORMAL;
  float2 texcoord: TEXCOORD0;
};

struct GsOutput {
  float4 position:SV_POSITION;
};

cbuffer vs_buffer : register(cb0) {
   float4x4 pv;
   float4x4 world;
   float linelength;
};

cbuffer ps_buffer : register(cb0) {
   float4 color;
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

[maxvertexcount(6)]
void gs_main(triangle VsOutput v[3], inout LineStream<GsOutput> linestream) {
  linestream.RestartStrip();

  GsOutput o;
  [unroll]
  for (int i = 0; i < 3; ++i) {
    o.position = v[i].position;
    linestream.Append(o);
    o.position = v[i].npos;
    linestream.Append(o);
    break;
  }
}

float4 ps_main(GsOutput o):SV_TARGET {
  return color;
}
