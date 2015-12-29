#pragma once

#include <vector>

#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/render/render_system_enum.h"
#include "base/logging.h"

namespace azer {

class Camera;
class AxisAlignedBoundingBox;

class AZER_EXPORT Frustum {
public:
  Frustum(Camera* camera);

  Frustum(Camera* camera, Radians fovy, float apsect,
           float z_near = 1.0f, float z_far = 1000.0f);
  Frustum& operator = (const Frustum& frustum);

  void set_fovy(Radians fovy) { fovY_ = fovy; GenProjMatrix();}
  void set_far(float _far) { far_ = _far; GenProjMatrix();}
  void set_near(float _near) { near_ = _near; GenProjMatrix();}
  void set_aspect(float aspect) { aspect_ = aspect; GenProjMatrix();}

  Radians fovy() const { return fovY_;}
  float aspect() const { return aspect_;}
  float get_near() const { return near_;}
  float get_far() const { return far_;}
  
  const Matrix4& projection() const { return projection_;}

  // index of plane
  enum {
    kNearPlane = 0,
    kFarPlane,
    kLeftPlane,
    kRightPlane,
    kTopPlane,
    kBottomPlane,
  };

  enum CheckVisibleOption {
    kCheckNearPlane      = 0x00000001,
    kCheckFarPlane       = 0x00000002,
    kCheckLeftPlane      = 0x00000004,
    kCheckRightPlane     = 0x00000008,
    kCheckTopPlane       = 0x00000010,
    kCheckBottomPlane    = 0x00000020,
    kCheckWithoutHeight  = 0x0000000F,
    kCheckAll            = 0x0000003F,
  };
  VisibleState IsVisible(const Vector3& point, CheckVisibleOption opt) const;
  VisibleState IsVisible(const Vector3& point) const;
  VisibleState IsVisible(const Vector3& center, const Vector3& halfsize) const;

  // recalc frustum plane of camera
  void UpdatePlane();
  const Camera* camera() const { return camera_;}
 private:
  void GenProjMatrix();
  Radians fovY_;
  float aspect_;
  float near_;
  float far_;
  Camera* camera_;  
  azer::Matrix4 projection_;

  std::vector<Plane> planes_;
};
}  // namespace azer
