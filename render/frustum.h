#pragma once

#include <vector>

#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/render/common.h"
#include "base/logging.h"

namespace azer {

class Camera;
class AxisAlignedBoundingBox;

enum {
  kOrthognalFrustum,
  kPerspectiveFrustum,
};

class AZER_EXPORT Frustum {
 public:
  // Perspective Projection
  // default Radians(kPI / 4.0f), aspect=4/3
  Frustum(Radians fovy, float apsect, float z_near = 1.0f, float z_far = 1000.0f);

  // Orthognal Projection
  Frustum(float width, float height, float znear, float zfar);
  Frustum(const Frustum &frustum);

  int type() const { return type_;}

  void set_fovy(Radians fovy);
  void set_far(float _far);
  void set_near(float _near);
  void set_aspect(float aspect);
  void set_width(float height);
  void set_height(float width);

  float znear() const { return near_;}
  float zfar() const { return far_;}
  Radians fovy() const { CHECK_EQ(type(), kPerspectiveFrustum); return fovY_;}
  float aspect() const { CHECK_EQ(type(), kPerspectiveFrustum); return aspect_;}
  float width() const { CHECK_EQ(type(), kOrthognalFrustum); return width_;}
  float height() const { CHECK_EQ(type(), kOrthognalFrustum); return height_;}
  
  void UpdateProjMatrix();
  const Matrix4& projection() const { return projection_;}
 private:
  Radians fovY_;
  float aspect_;
  float near_;
  float far_;
  float width_;
  float height_;
  const int type_;
  azer::Matrix4 projection_;
};
}  // namespace azer
