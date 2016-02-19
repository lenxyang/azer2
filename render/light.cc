#include "azer/base/light.h"

#include "base/logging.h"
#include "azer/render/light_observer.h"

namespace azer {
DirLight::DirLight()
    : factor(1.0f),
      type(kDirectionalLight) {
}

PointLight::PointLight() 
    : factor(1.0f),
      type(kPointLight) {
}

SpotLight::SpotLight() 
    : factor(1.0f),
      type(kSpotLight) {
}


namespace {
const Vector4 none_color(0.0f, 0.0f, 0.0f, 0.0f);
const Vector3 none_dir(0.0f, 0.0f, 0.0f);
}

Light::Light() {memset(&data_, 0, sizeof(data_));}
Light::Light(const Light& light) {
  *this = light;
}
Light& Light::operator = (const Light& light) {
  memset(&this->data_, &light->data_, sizeof(data_));
  return *this;
}
Light::Light(const DirLight& light) { SetLight(light);}
Light::Light(const PointLight& light) { SetLight(light);}
Light::Light(const SpotLight& light) { SetLight(light); }

void Light::SetLight(const DirLight& light) {
  memcpy(&data_, &light, sizeof(light));
}

void Light::SetLight(const PointLight& light) {
  memcpy(&data_, &light, sizeof(light));
}

void Light::SetLight(const SpotLight& light) {
  memcpy(&data_, &light, sizeof(light));
}

void Light::set_factor(bool v) {
  switch (type()) {
    case kDirectionalLight:
    case kPointLight:
    case kSpotLight:
      data_.factor = v;
      break;
    default:
      CHECK(false);
  }
}

void Light::set_diffuse(const Vector4& v) {
  switch (type()) {
    case kDirectionalLight:
    case kPointLight:
    case kSpotLight:
      memcpy(data_.diffuse, &v, sizeof(v));
      break;
    default:
      CHECK(false);
  }
}

void Light::set_ambient(const Vector4& v) {
  switch (type()) {
    case kDirectionalLight:
    case kPointLight:
    case kSpotLight:
      memcpy(data_.ambient, &v, sizeof(v));
      break;
    default:
      CHECK(false);
  }
}

void Light::set_specular(const Vector4& v) {
  switch (type()) {
    case kDirectionalLight:
    case kPointLight:
    case kSpotLight:
      memcpy(data_.specular, &v, sizeof(v));
      break;
    default:
      CHECK(false);
  }
}

void Light::set_position(const Vector3& pos) {
  switch (type()) {
    case kPointLight:
    case kSpotLight:
      memcpy(data_.position, &pos, sizeof(pos));
      break;
    case kDirectionalLight:
    default:
      CHECK(false);
  }
}

void Light::set_directional(const Vector3& dir) {
  switch (type()) {
    case kDirectionalLight:
    case kSpotLight:
      memcpy(data_.directional, &dir, sizeof(dir));
      break;
    case kPointLight:
    default:
      CHECK(false);
  }
}

int Light::type() const {
  return data_.type;
}

float Light::factor() const {
  switch (type()) {
    case kDirectionalLight:
    case kPointLight:
    case kSpotLight:
      return data_.factor;
    default:
      CHECK(false);
      return 1.0f;
  }
}

const Vector3& Light::position() const {
  return *(Vector3*)(&data_.position);
}

const Vector3& Light::directional() const {
  return *(Vector3*)(&data_.position);
}

const Vector4& Light::diffuse() const {
  return *(Vector4*)(&data_.diffuse);
}

const Vector4& Light::ambient() const {
  return *(Vector4*)(&data_.ambient);
}

const Vector4& Light::specular() const {
  return *(Vector4*)(&data_.specular);
}

void Light::set_phi(float v) {
  data_.spotarg.phi = v;
}

void Light::set_theta(float v) {
  data_.spotarg.theta = v;
}

void Light::set_falloff(float v) {
  data_.spotarg.falloff = v;
}

void Light::set_range(float v) {
  if (type() == kPointLight) {
    data_.attenuation.range = v;
  } else if (type() == kSpotLight) {
    data_.spotarg.range = v;
  } else {
    NOTREACHED();
  }
}

void Light::set_attenuation(const Vector3& v) {
  DCHECK(type() == kPointLight);
  memcpy(data_.attenuation.coeff, &v, sizeof(v));
}

// ObserverableLight
ObserverableLight::ObserverableLight() {
}

ObserverableLight::ObserverableLight(const Light& l) {
  SetLight(l);
}

void ObserverableLight::set_factor(bool v) {
  light_.set_factor(v);
}

void ObserverableLight::set_diffuse(const Vector4& color) {
  light_.set_diffuse(v);
}

void ObserverableLight::set_ambient(const Vector4& color) {
  light_.set_ambient(v);
}

void ObserverableLight::set_specular(const Vector4& color) {
  light_.set_specular(v);
}

void ObserverableLight::set_position(const Vector3& pos) {
  light_.set_position(v);
}

void ObserverableLight::set_directional(const Vector3& dir) {
  light_.set_directional(v);
}

void ObserverableLight::set_phi(float v) {
  light_.set_phi(v);
}

void ObserverableLight::set_theta(float v) {
  light_.set_theta(v);
}

void ObserverableLight::set_falloff(float v) {
  light_.set_falloff(v);
}

void ObserverableLight::set_range(float v) {
  light_.set_range(v);
}

void ObserverableLight::set_attenuation(const Vector3& v) {
  light_.set_attenuation(v);
}
}  // namespace azer
