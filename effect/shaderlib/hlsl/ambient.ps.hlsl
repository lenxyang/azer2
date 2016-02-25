struct VsOutput {
  float4 position:SV_POSITION;
  float3 worldpos: WPOS;
  float3 normal:NORMAL;
  float3 viewin: VIEWIN;
  float2 texcoord: TEXCOORD0;
};

cbuffer c_buffer {
   float4     ambient;
};

float4 ps_main(VsOutput o):SV_TARGET {
  return ambient;
}
