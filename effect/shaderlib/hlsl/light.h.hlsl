
#define DIRLIGHT 1
#define POINTLIGHT 2
#define SPOTLIGHT 3

struct Light {
  float4  diffuse;
  float4  ambient;
  float4  specular;
  float3  position;
  float   factor;   // light factor 
  float3  directional;
  int     type;
  // for spotlight: phi, thread, range, falloff
  // for pointlight: attenuation, range
  // for dirlight: nothing
  float4  args;
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

float phong(float3 ldir, float3 normal, float3 viewin) {
  float3 r = normalize(2.0f * dot(normal, ldir) * normal - ldir);
  return max(0, dot(r, viewin));
}

float blinn_phong(float3 ldir, float3 normal, float3 viewin) {
  float3 half_vec = normalize(ldir + viewin);
  return max(0, dot(half_vec, normal));
}

float3 CalcDirLightColor(Light light, float3 normal, float3 viewin, Material m) {
  float3 ldir = normalize(light.directional.xyz);
  float3 diffuse = max(0.0, dot(normal, -ldir)) * 
                  light.diffuse.xyz * m.diffuse.xyz;
  float3 ambient = m.ambient.xyz * light.ambient.xyz;
  float3 specular = pow(m.power, phong(ldir, normal, viewin))
                        * light.specular.xyz * m.specular.xyz;

  float3 c = float3(ambient.xyz + diffuse.xyz + specular.xyz + m.emission.xyz);
  return light.factor * c;
}

float CalcAttenuation(float dist, float3 atten, float range) {
  float att = 1.0f / (atten.x * dist * dist + atten.y * dist + atten.z);
  return step(dist, range) * saturate(att);
}

float3 CalcPointLightColor(Light light, float3 pos, float3 normal, 
                           float3 viewin, Material mtrl) {
  float3 ldir = normalize(pos - light.position);
  float dist = distance(pos, light.position);
  float3 diffuse = max(0.0, dot(normal, -ldir)) * 
                  light.diffuse.xyz * mtrl.diffuse.xyz;
  float atten = CalcAttenuation(dist, light.args.xyz, light.args.w);
  float3 ambient = mtrl.ambient.xyz * light.ambient.xyz;
  float3 specular = pow(mtrl.power, phong(ldir, normal, viewin))
                        * light.specular.xyz * mtrl.specular.xyz;

  float3 c = float3(ambient.xyz + (diffuse.xyz + specular.xyz) * atten
                    + mtrl.emission.xyz);
  return c * light.factor;
}

float3 CalcSpotLightColor(Light light, float3 pos, float3 normal, 
                          float3 viewin, Material m) {
  float spot_atten = 0.0f;
  float3 ldir = normalize(pos - light.position);
  float dist = distance(pos, light.position);
  float cosalpha = max(0.0f, dot(ldir, light.directional));
  float phi = light.args.x;
  float theta = light.args.y;
  float range = light.args.z;
  float falloff = light.args.w;
  if (cosalpha > theta) {
    spot_atten = 1.0f;
  } else if (cosalpha > phi) {
    float ratio = (cosalpha - phi) / (theta - phi);
    spot_atten = pow(ratio, falloff);
  }

  float linear_atten = lerp(1.0f, 0.0f, dist / range);
  float atten = linear_atten * spot_atten;
  float3 diffuse = max(0.0, dot(normal, -ldir)) * light.diffuse.xyz * m.diffuse.xyz;
  float3 ambient = m.ambient.xyz * light.ambient.xyz;
  float3 specular = pow(m.power, blinn_phong(ldir, normal, viewin))
                        * light.specular.xyz * m.specular.xyz;
  float3 c = ambient.xyz + (diffuse.xyz + specular.xyz) * atten + m.emission.xyz;
  return c * light.factor;
}

float3 CalcLight(Light l, float3 normal, float3 viewin, float3 worldpos, 
                 Material mtrl) {
  if (l.type == DIRLIGHT) {
    return CalcDirLightColor(l, normal, viewin, mtrl);
  } else if (l.type == POINTLIGHT) {
    return CalcPointLightColor(l, worldpos, normal, viewin, mtrl);
  } else if (l.type == SPOTLIGHT) {
    return CalcSpotLightColor(l, worldpos, normal, viewin, mtrl);
  } else {
    return float3(1.0f, 0.0f, 0.0f);
  }
}
