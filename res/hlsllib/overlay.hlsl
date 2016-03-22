
struct VsOutput {
  float4 pos      : SV_POSITION;
  float2 texcoord : TEXCOORD0; 
};

/* [0,1][0,1] */
cbuffer ps_buffer : register(cb0) {
  float left;
  float top;
  float width;
  float height;
};

static const float2 uv[4] = {
  float2(0.0f, 0.0f),
  float2(0.0f, 1.0f),
  float2(1.0f, 0.0f),
  float2(1.0f, 1.0f),
};

Texture2D tex : register(t0);
SamplerState sample_state;

VsOutput vs_main(uint vertex_id : SV_VertexID) {
  VsOutput output;
  float4 position[4] = {
    float4(left * 2 - 1.0f, 2 - top * 2, 0.0f, 0.0f),
    float4(left * 2 - 1.0f, 2 - (top + height) * 2, 0.0f, 0.0f),
    float4((left + width) * 2 - 1.0f, 2 - top * 2, 0.0f, 0.0f),
    float4((left + width) * 2 - 1.0f, 2 - (top + height) * 2, 0.0f, 0.0f),
  };
  output.pos = position[vertex_id];
  output.texcoord = uv[vertex_id];
  return output;
};


float4 ps_main(VsOutput input) : SV_TARGET {
  return tex.Sample(sample_state, input.texcoord);
};
