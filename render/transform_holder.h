#pragma once


#include "base/observer_list.h"
#include "azer/base/export.h"
#include "azer/math/math.h"

namespace azer {

class TransformHolder;
class AZER_EXPORT TransformHolderObserver {
 public:
  virtual void OnObjectPositionChanged(TransformHolder* object, 
                                       const Vector3& origin_position) = 0;
  virtual void OnObjectOrientationChanged(TransformHolder* object, 
                                          const Quaternion& origin_orientation) = 0;
};

class AZER_EXPORT TransformHolder {
 public:
  TransformHolder();
  TransformHolder(const Vector3& pos);
  TransformHolder(const Vector3& pos, const Quaternion& orient);
  TransformHolder(const TransformHolder& obj);
  virtual ~TransformHolder() {}
  
  TransformHolder& operator = (const TransformHolder& obj);
  
  void pitch(const Radians angle);
  void yaw(const Radians angle);
  void roll(const Radians angle);

  void pitch(const Degree angle);
  void yaw(const Degree angle);
  void roll(const Degree angle);

  void rotate(const Vector3& axis, const Degree angle);
  void rotate(const Vector3& axis, const Radians angle);
  void rotate(const Quaternion& q);

  // walk along with z axis
  // 很多设计当中, 提供了两种模式：
  // 一种是陆地模式，这种模式下，只能在陆地上按照 camera_z 在地平面的分量上行走
  // 另外一种是飞行模式，这种模式下，可以直接按照 camera_z 飞行
  //
  void fly(float step);
  void walk(float step);
  void strafe(float step);

  Vector3 right() const;
  Vector3 up() const;
  Vector3 directional() const;
  void set_orientation(const Quaternion& q);
  const Quaternion& orientation() const { return orientation_;}

  const Vector3& scale() const { return scale_;}
  void SetScale(const Vector3& v) { scale_ = v;}

  const Vector3& position() const { return position_;}
  void SetPosition(const Vector3& pos) { position_ = pos;}
  void SetPosition(float x, float y, float z) { position_ = Vector3(x, y, z);}

  Matrix4 GenWorldMatrix() const;

  virtual void OnPositionChanged(const Vector3& origin_position) {}
  virtual void OnOrientationChanged(const Quaternion& origin_orientation) {}
 protected:
  void PositionChanged(const Vector3& origin_position);
  void OrientationChanged(const Quaternion& origin_orientation);
  Vector3 position_;
  Vector3 scale_;
  Quaternion orientation_;
};

}  // namespace azer

