
struct VsOutput {
  float4 pos      : SV_POSITION;
  float2 texcoord : TEXCOORD0; 
};

/* [0,1][0,1] */
cbuffer vs_buffer : register(b0) {
  float4 bounds;
  float4 texbounds;
};

Texture2D tex : register(t0);
Texture2DMS<float4> mstex : register(t0);
SamplerState sample_state : register(s0);

VsOutput vs_main(uint vertex_id : SV_VertexID) {
  VsOutput output;
  float x = bounds.x;
  float y = bounds.y;
  float4 position[4] = {
    float4(x * 2 - 1.0f,              1 - y * 2,              0.0f, 1.0f),
    float4(x * 2 - 1.0f,              1 - (y + bounds.w) * 2, 0.0f, 1.0f),
    float4((x + bounds.z) * 2 - 1.0f, 1 - y * 2,              0.0f, 1.0f),
    float4((x + bounds.z) * 2 - 1.0f, 1 - (y + bounds.w) * 2, 0.0f, 1.0f),
  };
  float2 uv[4] = {
    float2(texbounds.x, texbounds.y),
    float2(texbounds.x, texbounds.z),
    float2(texbounds.w, texbounds.y),
    float2(texbounds.w, texbounds.z),
  };
  output.pos = position[vertex_id];
  output.texcoord = uv[vertex_id];
  return output;
};


float4 ps_main(VsOutput input) : SV_TARGET {
  return tex.Sample(sample_state, input.texcoord);
};

float4 ps_main_ms(VsOutput input) : SV_TARGET {
  return mstex.Load(input.texcoord, 0);
};
