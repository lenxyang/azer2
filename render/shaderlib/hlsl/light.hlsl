#pragma pack_matrix(row_major)

struct Light {
  float4  diffuse;
  float4  ambient;
  float4  specular;
  float4  position;
  float4  directional;
  float4  attenuation;
  float   phi;
  float   theta;
  float   range;
  float   falloff;
  int     type;
  float   enable;
  float2  padding;
};

struct Material {
  float4 diffuse;
  float4 ambient;
  float4 specular;
  float4 emission;
  float  power;
  float  alpha;
  float  pad1;
  float  pad2;
};

float CalcAttenuation(float dist, float4 atten) {
  float att = 1.0f / (atten.x * dist * dist + atten.y * dist + atten.z);
  return step(dist, atten.range) * saturate(att);
}
