#pragma once

#include "azer/base/export.h"
#include "azer/math/math.h"
#include "base/basictypes.h"

namespace azer {

class AZER_EXPORT MovableObject {
 public:
  class Delegate {
   public:
    virtual void OnObjectPositionChanged(const Vector3& origin_position) = 0;
    virtual void OnObjectOrientationChanged(const Quaternion& origin_orientation) = 0;
  };

  MovableObject();
  MovableObject(const Vector3& pos);
  MovableObject(const Vector3& pos, const Quaternion& orient);
  MovableObject(const MovableObject& obj);
  virtual ~MovableObject() {}

  void set_delegate(Delegate* delegate) { MovableObject::delegate_ = delegate;}
  Delegate* delegate() { return MovableObject::delegate_;}

  MovableObject& operator = (const MovableObject& obj);
  
  void pitch(const Radians angle);
  void yaw(const Radians angle);
  void roll(const Radians angle);

  void pitch(const Degree angle);
  void yaw(const Degree angle);
  void roll(const Degree angle);

  void rotate(const Vector3& axis, const Degree angle);
  void rotate(const Vector3& axis, const Radians angle);
  void rotate(const Quaternion& q);
  void set_rotate(const Quaternion& q);

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
  Vector3 direction() const;
  const Quaternion& orientation() const { return orientation_;}

  const Vector3& position() const { return position_;}
  void SetPosition(const Vector3& pos) { position_ = pos;}
  void SetPosition(float x, float y, float z) { position_ = Vector3(x, y, z);}
  Matrix4 world() const;
 protected:
  Vector3 position_;
  Quaternion orientation_;
  Delegate* delegate_;
};

}  // namespace azer

