

cbuffer hsbuffer : register(b0) {
  float4 edge;
  float4 inside;
};

cbuffer hsbuffer : register(b0) {
  float4x4 pvw;
  float4x4 world;
};

cbuffer psbuffer {
  float4 color;
};

struct VSInput {
  float4 position:POSITION;
};

struct VsOutput {
  float4 position: SV_POSITION;
};

struct HSCOutput {
  float edge[4]: SV_TessFactor;
  float inside[2]:  SV_InsideTessFactor;
};

struct HsOutput {
  float4 position: POSITION;
};

struct DsOutput {
  float4 position: SV_POSITION;
};

VsOutput vs_main(VSInput input) {
  VsOutput o;
  o.position = input.position;
  return o;
};

HSCOutput PatchConstantFunc(InputPatch<VsOutput, 4> input, 
                            uint patchid : SV_PrimitiveID) {
  HSCOutput output;
  output.edge[0] = edge.x;
  output.edge[1] = edge.y;
  output.edge[2] = edge.z;
  output.edge[3] = edge.w;
  output.inside[0] = inside.x;
  output.inside[1] = inside.y;
  return output;
}
[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunc")]
[maxtessfactor(64.0f)]
HsOutput hs_main(InputPatch<VsOutput, 4> patch, 
                 uint pointid: SV_OutputControlPointID, 
                 uint patchid: SV_PrimitiveID) {
  HsOutput output;
  output.position = patch[pointid].position;
  return output;
};


[domain("quad")]
DsOutput ds_main(HSCOutput input, 
                 const OutputPatch<HsOutput, 4> quad, 
                 float2 uv : SV_DomainLocation) {
  DsOutput output;
  float3 v1 = lerp(quad[0].position.xyz, quad[1].position.xyz, uv.x);
  float3 v2 = lerp(quad[3].position.xyz, quad[2].position.xyz, uv.x);
  output.position = mul(pvw, float4(lerp(v1, v2, uv.y), 1.0f));
  return output;
};

float4 ps_main(DsOutput o):SV_TARGET {
  return color;
};
