#pragma once


#include "azer/base/export.h"
#include "azer/math/math.h"
#include "azer/render/frustum.h"
#include "azer/render/movable.h"
#include "azer/render/transform_holder.h"

namespace azer {

/**
 * Camera 的实现思路
 * 默认情况下 Camera 的位置在 (0, 0, 0, 1), 面向 -z 方向
 * Camera 使用两个向量表示 position 和 orientation(Q)
 * 其中 orientation 使用四元数表示，它表示如何将默认的 Camera 旋转至当前方向
 * 因此确定的各轴方式的是 Q * axis * Q^-1, 此运算 Quaternion 提供了直接的方法
 *
 */
class AZER_EXPORT Camera {
 public:
  Camera();
  explicit Camera(const Frustum& frustum);
  Camera(float width, float height, float znear, float zfar);
  Camera(const Vector3& pos, const Vector3& lookat, const Vector3& up);

  void reset(const Vector3& pos, const Vector3& lookat, const Vector3& up);

  void SetLookAt(const Vector3& lookat);
  void SetDirection(const Vector3& dir);
  void SetOrientation(const Quaternion& orientation);

  const Matrix4& GetViewMatrix() const { return view_mat_;}
  const Matrix4& GetProjViewMatrix() const { return proj_view_mat_;}

  const Vector3& position() const;
  Vector3 right() const;
  Vector3 up() const;
  Vector3 directional() const;
  const Quaternion& orientation() const;

  void Update();
  TransformHolder* mutable_holder() { return &holder_;}
  const TransformHolder& holder() const { return holder_;}

  const Frustum& frustum() const { return frustum_;}
  Frustum* mutable_frustum() { return &frustum_;}
  friend std::ostream& operator << (std::ostream& os, const Camera& camera);
 protected:
  // generate camera transform matrix
  // the matrix will transform the object from world-cord to camera-cord
  // Note: the view matrix is RIGHT-HAND  
  virtual void GenMatrices();
  void UpdateDirection();

  Matrix4 view_mat_;
  Matrix4 proj_view_mat_;
  Frustum frustum_;
  TransformHolder holder_;
};

class AZER_EXPORT CameraCullingHelper {
 public:
  explicit CameraCullingHelper(const Camera& camera);
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
 private:
  const Camera& camera_;
  std::vector<Plane> planes_;
  DISALLOW_COPY_AND_ASSIGN(CameraCullingHelper);
};

inline std::ostream& operator << (std::ostream& os, const Camera& camera) {
  const TransformHolder& holder = camera.holder();
  os << "azer::camera info{ dir:" << holder.directional()
     << ", up: " << holder.up()
     << ", pos: " << holder.position() << "}";
  return os;
}

AZER_EXPORT void CalcCameraBundingPos(const Camera& camera, float znear, float zfar, 
                                      Vector3 pos[8]);
AZER_EXPORT void CalcCameraBundingPos(const Camera& camera, Vector3 pos[8]);
AZER_EXPORT void CalcCameraAABB(const Camera& camera, float znear, float zfar,
                                Vector3* vmin, Vector3* vmax);
}  // namespace azer

