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
        camera.holder_.position() + camera.holder_.directional(),
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
  
  view_mat_ = std::move(LookDirRH(holder_.position(), holder_.directional(), 
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

Vector3 Camera::directional() const {
  return std::move(holder().directional());
}

const Quaternion& Camera::orientation() const {
  return holder().orientation();
}

void CalcCameraBundingPos(const Camera& camera, float znear, float zfar, 
                          Vector3 pos[8]) {
  const Matrix4 vmat = camera.GetViewMatrix();
  float aspect = camera.frustum().aspect();
  Degree fovy = Degree(camera.frustum().fovy());
  float fTanFOVY = azer::tan(fovy);
  float fTanFOVX = aspect * fTanFOVY;
  Vector3 cdir = camera.directional();
  Vector3 right = camera.right();
  Vector3 up = camera.up();
  Vector3 cpos = camera.position();
  pos[0] = cpos + (-right * fTanFOVX + up * fTanFOVY + cdir) * znear;
  pos[1] = cpos + ( right * fTanFOVX + up * fTanFOVY + cdir) * znear;
  pos[2] = cpos + ( right * fTanFOVX - up * fTanFOVY + cdir) * znear;
  pos[3] = cpos + (-right * fTanFOVX - up * fTanFOVY + cdir) * znear;

  pos[4] = cpos + (-right * fTanFOVX + up * fTanFOVY + cdir) * zfar;
  pos[5] = cpos + ( right * fTanFOVX + up * fTanFOVY + cdir) * zfar;
  pos[6] = cpos + ( right * fTanFOVX - up * fTanFOVY + cdir) * zfar;
  pos[7] = cpos + (-right * fTanFOVX - up * fTanFOVY + cdir) * zfar;
}

void CalcCameraBundingPos(const Camera& camera, Vector3 pos[8]) {
  CalcCameraBundingPos(camera, camera.frustum().get_near(), 
                       camera.frustum().get_far(), pos);
}

void CalcCameraBundingBox(const Camera& camera, float znear, float zfar,
                          Vector3* vmin, Vector3* vmax) {
  Vector3 pos[8];
  CalcCameraBundingPos(camera, znear, zfar, pos);
  azer::InitMinAndVMax(vmin, vmax);
  for (uint32_t i = 0; i < arraysize(pos); ++i) {
    azer::UpdateVMinAndVMax(pos[i], vmin, vmax);
  }
}
}  // namespace azer
