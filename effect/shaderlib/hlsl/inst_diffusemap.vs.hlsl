struct VsOutput {
  float4 position:SV_POSITION;
  float3 worldpos: WPOS;
  float3 normal:NORMAL;
  float3 viewin: VIEWIN;
  float2 texcoord: TEXCOORD0;
};

struct VSInput {
  float3 position:POSITION;
  float3 normal:NORMAL;
  float2 texcoord: TEXCOORD0;
  float4 world0: WORLD0;
  float4 world1: WORLD1;
  float4 world2: WORLD2;
  float4 world3: WORLD3;
//  float4x4 world: WORLD;
};

cbuffer c_buffer {
   float4x4 pv;
   float4x4 pworld;
   float4   camerapos;
};

VsOutput vs_main(VSInput v) {
  VsOutput o;
  float4x4 world = float4x4(v.world0, v.world1, v.world2, v.world3);
      
  o.worldpos = mul(world, float4(v.position, 1.0)).xyz;
  o.position = mul(pv, float4(o.worldpos, 1.0));
  o.normal = normalize(mul(world, float4(v.normal, 0.0)).xyz);
  o.viewin = normalize(camerapos - o.worldpos);
  o.texcoord = v.texcoord;
  return o;
}
