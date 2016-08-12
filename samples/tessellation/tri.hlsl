

cbuffer vs_buffer : register(b0) {
  float4x4 pvw;
  float4x4 world;
};
cbuffer hs_buffer : register(b0) { 
  float4 edge;
};

cbuffer ps_buffer : register(b0) {
  float4 color;
};

struct VsOutput {
  float4 position:SV_POSITION;
};
struct VSInput {
  float4 position:POSITION;
};

struct HSCOutput {
  float edge[3]: SV_TessFactor;
  float inside:  SV_InsideTessFactor;
};

struct HsOutput {
  float4 position: POSITION;
};

struct DsOutput {
  float4 position:SV_POSITION;
};

VsOutput vs_main(VSInput input) {
  VsOutput o;
  o.position = mul(pvw, input.position);
  return o;
};

HSCOutput PatchConstantFunc(InputPatch<VsOutput, 3> input, 
                            uint patchid : SV_PrimitiveID) {
  HSCOutput output;
  output.edge[0] = edge.x;
  output.edge[1] = edge.y;
  output.edge[2] = edge.z;
  output.inside = edge.w;
  return output;
}
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunc")]
HsOutput hs_main(InputPatch<VsOutput, 3> patch, 
                 uint pointid: SV_OutputControlPointID, 
                 uint patchid: SV_PrimitiveID) {
  HsOutput output;
  output.position = patch[pointid].position;
  return output;
};

[domain("tri")]
DsOutput ds_main(HSCOutput input, float3 uvw : SV_DomainLocation,
                     const OutputPatch<HsOutput, 3> patch) {
  DsOutput output;
  output.position = uvw.x * patch[0].position
      + uvw.y * patch[1].position
      + uvw.z * patch[2].position;
  return output;
};


float4 ps_main(DsOutput o):SV_TARGET {
  return color;
};
