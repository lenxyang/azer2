#pragma once

#include <vector>
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "azer/render/render.h"

namespace azer {
class Light;
typedef scoped_refptr<Light> LightPtr;
typedef std::vector<LightPtr> Lights;
class LightObserver;

enum LightType {
  kUnknonwLight     = 0,
  kDirectionalLight = 1,
  kPointLight       = 2,
  kSpotLight        = 3,
};

struct AZER_EXPORT DirLight {
  Vector4 diffuse;
  Vector4 ambient;
  Vector4 specular;
  Vector3 internal_position;  // ignore
  float factor;
  Vector3 directional;
  const int type;             // ignore
  DirLight();
};

struct AZER_EXPORT PointLight {
  Vector4 diffuse;
  Vector4 ambient;
  Vector4 specular;
  Vector3 position;
  float factor;
  Vector3 internal_directional;  // ignore
  const int type;                // ignore, just for memory
  Vector3 coeff;
  float range;
  PointLight();
};

struct AZER_EXPORT SpotLight {
  Vector4 diffuse;
  Vector4 ambient;
  Vector4 specular;
  Vector3 position;
  float factor;
  Vector3 directional;
  int type;
  float phi;
  float theta;  // inner corner (cosine)
  float range;
  float falloff;

  SpotLight();
};

typedef float float2[2];
typedef float float3[3];
typedef float float4[4];
struct UniverseLight {
  float4 diffuse;
  float4 ambient;
  float4 specular;
  float3 position;
  float  factor;
  float3 directional;
  int    type;
  union {
    // for spot light
    struct {
      float phi;
      float theta;
      float falloff;
      float range;
    } spotarg;
      
    // for point light
    struct {
      float3 coeff;
      float range;
    } attenuation;
  };
};


class AZER_EXPORT Light : public ::base::RefCounted<Light> {
 public:
  Light();
  explicit Light(const DirLight& light);
  explicit Light(const PointLight& light);
  explicit Light(const SpotLight& light);

  int32 type() const;
  const UniverseLight& data() const { return data_;}

  void SetLight(const DirLight& light);
  void SetLight(const PointLight& light);
  void SetLight(const SpotLight& light);
  
  const Vector3& position() const;
  const Vector3& directional() const;
  const Vector4& diffuse() const;
  const Vector4& ambient() const;
  const Vector4& specular() const;

  float factor() const;
  void set_factor(bool v);
  void set_diffuse(const Vector4& color);
  void set_ambient(const Vector4& color);
  void set_specular(const Vector4& color);
  void set_position(const Vector3& pos);
  void set_directional(const Vector3& dir);
  void set_phi(float v);
  void set_theta(float v);
  void set_falloff(float v);
  void set_range(float v);
  void set_attenuation(const Vector3& v);

  void AddObserver(LightObserver* observer);
  void RemoveObserver(LightObserver* observer);
  bool HasObserver(LightObserver* observer) const;
 private:
  UniverseLight data_;
  ObserverList<LightObserver> observers_;
  DISALLOW_COPY_AND_ASSIGN(Light);
};

}  // namespace azer
