#include "azer/render/frustum.h"

#include "azer/render/camera.h"
#include "base/logging.h"

namespace azer {

Frustum::Frustum() 
    : fovY_(Radians(kPI * 4)),
      aspect_(4.0f / 3.0f),
      near_(1.0f),
      far_(1000.0f),
      width_(1.0f),
      height_(1.0f),
      type_(kUnknownFrustum),
      modified_(false) {
}

Frustum::Frustum(Radians fovy, float apsect, float z_near, float z_far)
    : fovY_(fovy),
      aspect_(apsect),
      near_(z_near),
      far_(z_far),
      width_(1.0f),
      height_(1.0f),
      type_(kPerspectiveFrustum),
      modified_(false) {
  UpdateProjMatrix();
}

Frustum::Frustum(float width, float height, float znear, float zfar)
    : fovY_(Radians(10.0f)),
      aspect_(1.0f),
      near_(znear),
      far_(zfar),
      width_(width),
      height_(height),
      type_(kOrthognalFrustum),
      modified_(false) {
  UpdateProjMatrix();
}

Frustum::Frustum(const Frustum &frustum) {
  *this = frustum;
}

Frustum& Frustum::operator = (const Frustum &frustum) {
  fovY_ = frustum.fovY_;
  aspect_ = frustum.aspect_;
  near_ = frustum.near_;
  far_ = frustum.far_;
  width_ = frustum.width_;
  height_ = frustum.height_;
  type_ = frustum.type_;
  projection_ = frustum.projection_;
  modified_ = frustum.modified_;
  return *this;
}

void Frustum::UpdateProjMatrix() {
  switch (type()) {
    case kPerspectiveFrustum: 
      projection_ = std::move(PerspectiveRHD3D(fovY_, aspect_, near_, far_));
      break;
    case kOrthognalFrustum:
      projection_ = std::move(OrthoProjRH(width_, height_, near_, far_));
      break;
    default:
      CHECK(false) << "Unknown Frustum type: " << type();
      break;
  }
  modified_ = false;
}

void Frustum::set_far(float _far) { 
  far_ = _far; 
  modified_ = true;
  UpdateProjMatrix();
}

void Frustum::set_near(float _near) { 
  near_ = _near; 
  modified_ = true;
  UpdateProjMatrix();
}

void Frustum::set_fovy(Radians fovy) { 
  CHECK_EQ(type(), kPerspectiveFrustum);
  fovY_ = fovy; 
  modified_ = true;
  UpdateProjMatrix();
}

void Frustum::set_aspect(float aspect) { 
  CHECK_EQ(type(), kPerspectiveFrustum);
  aspect_ = aspect; 
  modified_ = true;
  UpdateProjMatrix();
}

void Frustum::set_width(float height) { 
  CHECK_EQ(type(), kOrthognalFrustum); 
  height_ = height; 
  modified_ = true;
  UpdateProjMatrix();
}
void Frustum::set_height(float width) { 
  CHECK_EQ(type(), kOrthognalFrustum); 
  width_ = width; 
  modified_ = true;
  UpdateProjMatrix();
}

}  // namespace azer
