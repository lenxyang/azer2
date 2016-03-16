
struct VsInput {
  float3 position: POSITION;
  float3 normal:   NORMAL;
  float2 texcoord: TEXCOORD0;
};

struct GsOutput {
  float4 position: SV_POSITION;
  float3 normal:   NORMAL;
  float2 texcoord: TEXCOORD0;
};

cbuffer c_buffer {
  float4x4 pv;
  float3 viewup;
  float3 viewpos;
  float2 params;
};

[maxvertexcount(30)]
void gs_main(point VsInput v[1], inout TriangleStream<GsOutput> stream) {
  float3 viewz = normalize(v[0].position - viewpos);
  float3 viewy = normalize(viewup);
  float3 viewx = normalize(cross(viewy, viewz));
  
  GsOutput o;
  float3 center = v[0].position;
  float3 pos;
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

  pos = center + params.x * viewx + params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(1.0f, 0.0f);
  stream.Append(o);
  stream.RestartStrip();

  pos = center - params.x * viewx + params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(0.0f, 0.0f);
  stream.Append(o);

  pos = center - params.x * viewx - params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(0.0f, 1.0f);
  stream.Append(o);

  pos = center + params.x * viewx - params.y * viewy;
  o.position = mul(pv, float4(pos, 1.0f));
  o.normal = viewz;
  o.texcoord = float2(1.0f, 1.0f);
  stream.Append(o);
  stream.RestartStrip();
}
