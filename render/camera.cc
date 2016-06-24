#include "azer/render/camera.h"

#include <iostream>
#include "azer/math/math.h"
#include "azer/render/bounding_volumn.h"

namespace azer {

Camera::Camera() : frustum_(this) {
  Update();
}

Camera::Camera(const Frustum& frustum)
    : frustum_(this, frustum.fovy(), frustum.aspect(), frustum.get_near(),
                frustum.get_far()) {
  Update();
}

Camera::Camera(const Vector3& pos)
    : frustum_(this) {
  Update();
}

Camera::Camera(const Vector3& pos, const Vector3& lookat,
               const Vector3& up)
    : frustum_(this) {
  reset(pos, lookat, up);
}

Camera& Camera::operator = (const Camera& camera) {
  reset(camera.holder_.position(),
        camera.holder_.position() + camera.holder_.direction(),
        camera.holder_.up());
  frustum_ = camera.frustum();
  return *this;
}

void Camera::reset(const Vector3& pos, const Vector3& lookat, const Vector3& up) {
  Matrix4 mat = LookAtRH(pos, lookat, up);
  holder_.SetPosition(pos);
  Vector3 xaxis(mat[0][0], mat[0][1], mat[0][2]);
  Vector3 yaxis(mat[1][0], mat[1][1], mat[1][2]);
  Vector3 zaxis(mat[2][0], mat[2][1], mat[2][2]);
  holder_.set_orientation(Quaternion::FromAxis(xaxis, yaxis, zaxis));
  Update();
}

void Camera::GenMatrices() {

  // TODO
  // View matrix is:
  //
  //  [ Lx  Uy  Dz  Tx  ]
  //  [ Lx  Uy  Dz  Ty  ]
  //  [ Lx  Uy  Dz  Tz  ]
  //  [ 0   0   0   1   ]
  //
  // Where T = -(Transposed(Rot) * Pos)
  /*
  Matrix4 view;
  Matrix4 rotT = std::move(orientation_.ToMatrix());
  Vector4 trans = std::move(-rotT * Vector4(pos_, 1.0f));
  view = rotT;
  view[0][3] = trans.x;
  view[1][3] = trans.y;
  view[2][3] = trans.z;
  */
  
  view_mat_ = std::move(LookDirRH(holder_.position(), holder_.direction(), 
                                  holder_.up()));
  proj_view_mat_ = std::move(frustum_.projection() * view_mat_);
}


void Camera::SetLookAt(const Vector3& lookat) {
  reset(holder_.position(), lookat, holder_.up());
}

void Camera::SetDirection(const Vector3& dir) {
  // do nothing
  if (dir.length() == 0) { return;}

  // adjust for Right hand
  DCHECK_FLOAT_EQ(dir.length(), 1.0f);
  Vector3 zaxis = -dir;
  CHECK(false);
}

void Camera::Update() {
  GenMatrices();
  frustum_.UpdatePlane();
}

const Vector3& Camera::position() const {
  return holder().position();
}

Vector3 Camera::right() const {
  return std::move(holder().right());
}

Vector3 Camera::up() const {
  return std::move(holder().up());
}

Vector3 Camera::direction() const {
  return std::move(holder().direction());
}

const Quaternion& Camera::orientation() const {
  return holder().orientation();
}

void CalcCameraBundingBox(const Camera& camera, float ffar, float fnear,
                          float aspect, float fov, Vector3* vmin, Vector3* vmax) {
  const Matrix4 vmat = camera.GetViewMatrix();
  float fTanFOVX = std::tan(aspect * fov);
  float fTanFOVY = std::tan(aspect);
  Vector3 pos[8];
  Vector3 cdir = camera.direction();
  pos[0] = camera.position() + 
      (-camera.right() * fTanFOVX + camera.up() * fTanFOVY + cdir) * fnear;
  pos[1] = camera.position() + 
      ( camera.right() * fTanFOVX + camera.up() * fTanFOVY + cdir) * fnear;
  pos[2] = camera.position() + 
      ( camera.right() * fTanFOVX - camera.up() * fTanFOVY + cdir) * fnear;
  pos[3] = camera.position() + 
      (-camera.right() * fTanFOVX - camera.up() * fTanFOVY + cdir) * fnear;

  pos[4] = camera.position() + 
      (-camera.right() * fTanFOVX + camera.up() * fTanFOVY + cdir) * ffar;
  pos[5] = camera.position() + 
      ( camera.right() * fTanFOVX + camera.up() * fTanFOVY + cdir) * ffar;
  pos[6] = camera.position() + 
      ( camera.right() * fTanFOVX - camera.up() * fTanFOVY + cdir) * ffar;
  pos[7] = camera.position() + 
      (-camera.right() * fTanFOVX - camera.up() * fTanFOVY + cdir) * ffar;

  
  azer::InitMinAndVMax(vmin, vmax);
  for (uint32_t i = 0; i < arraysize(pos); ++i) {
    azer::UpdateVMinAndVMax(pos[i], vmin, vmax);
  }
}
}  // namespace azer
