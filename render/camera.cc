#include "azer/render/camera.h"

#include <iostream>
#include "azer/math/math.h"
#include "azer/render/bounding_volumn.h"

namespace azer {

Camera::Camera() 
    : frustum_(Radians(kPI / 4.0f), 4.0f /3.0f, 1.0f, 1000.0f) {
  Update();
}

Camera::Camera(float width, float height, float znear, float zfar) 
    : frustum_(1.0f, 1.0f, 1.0f, 1000.0f) {
}

Camera::Camera(const Frustum& frustum)
    : frustum_(frustum) {
  Update();
}

Camera::Camera(const Vector3& pos, const Vector3& lookat, const Vector3& up)
    : frustum_(Radians(kPI / 4.0f), 4.0f /3.0f, 1.0f, 1000.0f) {
  reset(pos, lookat, up);
}

Camera& Camera::operator = (const Camera& camera) {
  view_mat_ = camera.view_mat_;
  proj_view_mat_ = camera.proj_view_mat_;
  frustum_ = camera.frustum_;
  holder_ = camera.holder_;
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

// class CameraCullingHelper
CameraCullingHelper::CameraCullingHelper(const Camera& camera)
    : camera_(camera) {
  planes_.resize(6);
}

void CameraCullingHelper::UpdatePlane() {
  const Matrix4& combo = camera_.GetProjViewMatrix();
  
  planes_[kNearPlane].normal().x = combo[3][0] + combo[2][0];
  planes_[kNearPlane].normal().y = combo[3][1] + combo[2][1];
  planes_[kNearPlane].normal().z = combo[3][2] + combo[2][2];
  planes_[kNearPlane].d()        = combo[3][3] + combo[2][3];

  planes_[kFarPlane].normal().x = combo[3][0] - combo[2][0];
  planes_[kFarPlane].normal().y = combo[3][1] - combo[2][1];
  planes_[kFarPlane].normal().z = combo[3][2] - combo[2][2];
  planes_[kFarPlane].d()        = combo[3][3] - combo[2][3];

  planes_[kLeftPlane].normal().x = combo[3][0] + combo[0][0];
  planes_[kLeftPlane].normal().y = combo[3][1] + combo[0][1];
  planes_[kLeftPlane].normal().z = combo[3][2] + combo[0][2];
  planes_[kLeftPlane].d()        = combo[3][3] + combo[0][3];

  planes_[kRightPlane].normal().x = combo[3][0] - combo[0][0];
  planes_[kRightPlane].normal().y = combo[3][1] - combo[0][1];
  planes_[kRightPlane].normal().z = combo[3][2] - combo[0][2];
  planes_[kRightPlane].d()        = combo[3][3] - combo[0][3];

  planes_[kTopPlane].normal().x = combo[3][0] - combo[1][0];
  planes_[kTopPlane].normal().y = combo[3][1] - combo[1][1];
  planes_[kTopPlane].normal().z = combo[3][2] - combo[1][2];
  planes_[kTopPlane].d()        = combo[3][3] - combo[1][3];

  planes_[kBottomPlane].normal().x = combo[3][0] + combo[1][0];
  planes_[kBottomPlane].normal().y = combo[3][1] + combo[1][1];
  planes_[kBottomPlane].normal().z = combo[3][2] + combo[1][2];
  planes_[kBottomPlane].d()        = combo[3][3] + combo[1][3];

  for (int i = 0; i < 6; ++i) {
    planes_[i].Normalize();
  }
}

VisibleState CameraCullingHelper::IsVisible(const Vector3& point) const {
  return IsVisible(point, kCheckAll);
}

VisibleState CameraCullingHelper::IsVisible(const Vector3& point,
                                            CheckVisibleOption opt) const {
  for (int i = 0; i < 6; ++i) {
    if (opt & (1 << 0)) {
      if (planes_[i].GetSide(point) == Plane::kNegative) {
        return kNoneVisible;
      }
    }
  }

  return kFullyVisible;
}

VisibleState CameraCullingHelper::IsVisible(const Vector3& center,
                                            const Vector3& halfsize) const {
  for (int i = 0; i < 6; ++i) {
    if (planes_[i].GetSide(center, halfsize) == Plane::kNegative) {
      return kNoneVisible;
    }
  }

  return kPartialVisible;
}

void CalcPropectiveCameraBoundsPox(const Camera& camera, Vector3 pos[8]) {
  CHECK_EQ(camera.frustum().type(), kPerspectiveFrustum);
  float zfar = camera.frustum().zfar();
  float znear = camera.frustum().znear();
  float tan_fovy = 0.0f, tan_fovx = 0.0f;
  float aspect = camera.frustum().aspect();
  Degree fovy = Degree(camera.frustum().fovy());
  tan_fovy = azer::tan(fovy);
  tan_fovx = aspect * tan_fovy;

  Vector3 cdir = camera.directional();
  Vector3 right = camera.right();
  Vector3 up = camera.up();
  Vector3 cpos = camera.position();
  pos[0] = cpos + (-right * tan_fovx + up * tan_fovy + cdir) * znear;
  pos[1] = cpos + ( right * tan_fovx + up * tan_fovy + cdir) * znear;
  pos[2] = cpos + ( right * tan_fovx - up * tan_fovy + cdir) * znear;
  pos[3] = cpos + (-right * tan_fovx - up * tan_fovy + cdir) * znear;

  pos[4] = cpos + (-right * tan_fovx + up * tan_fovy + cdir) * zfar;
  pos[5] = cpos + ( right * tan_fovx + up * tan_fovy + cdir) * zfar;
  pos[6] = cpos + ( right * tan_fovx - up * tan_fovy + cdir) * zfar;
  pos[7] = cpos + (-right * tan_fovx - up * tan_fovy + cdir) * zfar;
}

void CalcOrthognalCameraBoundsPox(const Camera& camera, Vector3 pos[8]) {
  CHECK_EQ(camera.frustum().type(), kOrthognalFrustum);
  float zfar = camera.frustum().zfar();
  float znear = camera.frustum().znear();
  Vector3 cdir = camera.directional();
  Vector3 right = camera.right();
  Vector3 up = camera.up();
  Vector3 cpos = camera.position();
  float width = camera.frustum().width();
  float height = camera.frustum().height();
  Vector3 nearpos = cpos + cdir * znear;
  Vector3 farpos = cpos + cdir * zfar;
  pos[0] = nearpos + (-right * width + up * height + cdir);
  pos[1] = nearpos + ( right * width + up * height + cdir);
  pos[2] = nearpos + ( right * width - up * height + cdir);
  pos[3] = nearpos + (-right * width - up * height + cdir);

  pos[4] = farpos + (-right * width + up * height + cdir);
  pos[5] = farpos + ( right * width + up * height + cdir);
  pos[6] = farpos + ( right * width - up * height + cdir);
  pos[7] = farpos + (-right * width - up * height + cdir);
}

void CalcCameraBundingPos(const Camera& camera, Vector3 pos[8]) {
  if (camera.frustum().type() == kPerspectiveFrustum) {
    CalcPropectiveCameraBoundsPox(camera, pos);
  } else {
    CalcOrthognalCameraBoundsPox(camera, pos);
  }
}

void CalcCameraAABB(const Camera& camera, Vector3* vmin, Vector3* vmax) {
  Vector3 pos[8];
  CalcCameraBundingPos(camera, pos);
  azer::InitMinAndVMax(vmin, vmax);
  for (uint32_t i = 0; i < arraysize(pos); ++i) {
    azer::UpdateVMinAndVMax(pos[i], vmin, vmax);
  }
}
}  // namespace azer
