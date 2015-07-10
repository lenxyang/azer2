#include "azer/render/movable.h"

namespace azer {

MovableObject::MovableObject()
    : position_(0.0f, 0.0f, 0.0f)
    , orientation_(Vector3(0.0, 1.0, 0.0), Degree(0.0f)) {}

MovableObject::MovableObject(const Vector3& position)
    : position_(position)
    , orientation_(Vector3(0.0, 1.0, 0.0), Degree(0.0f)) {
}

MovableObject::MovableObject(const Vector3& pos, const Quaternion& orient)
    : position_(pos)
    , orientation_(orient) {
}

MovableObject::MovableObject(const MovableObject& obj)
    : position_(obj.position())
    , orientation_(obj.orientation_) {
}

MovableObject& MovableObject::operator = (const MovableObject& obj) {
  position_ = obj.position();
  orientation_ = obj.orientation_;
  return *this;
}


void MovableObject::pitch(const Degree angle) {
  pitch(Radians(angle));
}

void MovableObject::yaw(const Degree angle) {
  yaw(Radians(angle));
}

void MovableObject::roll(const Degree angle) {
  roll(Radians(angle));
}

void MovableObject::pitch(const Radians angle) {
  Vector3 axis = orientation_ * Vector3::kUnitX;
  rotate(axis, angle);
}

void MovableObject::yaw(const Radians angle) {
  Vector3 axis = orientation_ * Vector3::kUnitY;
  rotate(axis, angle);
}

void MovableObject::roll(const Radians angle) {
  Vector3 axis = orientation_ * Vector3::kUnitZ;
  rotate(axis, angle);
}

void MovableObject::rotate(const Vector3& axis, const Radians rad) {
  Quaternion q(axis, rad);
  rotate(q);
}

void MovableObject::rotate(const Vector3& axis, const Degree angle) {
  Quaternion q(axis, angle);
  rotate(q);
}

void MovableObject::set_orientation(const Quaternion& q) {
  Quaternion origin = orientation_;
  orientation_ = q;
  orientation_.Normalize();
  if (orientation_ != origin) {
    ObjectOrientationChanged(origin);
  }
}

void MovableObject::rotate(const Quaternion& q) {
  DCHECK_FLOAT_EQ(q.length(), 1.0f);
  Quaternion origin = orientation_;
  orientation_ = q * orientation_;
  orientation_.Normalize();
  ObjectOrientationChanged(origin);
}

void MovableObject::walk(float step) {
  Vector3 position = position_;
  position_ +=  direction() * step;
  ObjectPositionChanged(position);
}

void MovableObject::fly(float step) {
  Vector3 position = position_;
  position_ += up() * step;
  ObjectPositionChanged(position);
}

void MovableObject::strafe(float step) {
  Vector3 position = position_;
  position_ += right() * step;
  bjectPositionChanged(position);
}

Vector3 MovableObject::right() const {
  return orientation_ * Vector3::kUnitX;
}

Vector3 MovableObject::up() const {
  return orientation_ * Vector3::kUnitY;
}

Vector3 MovableObject::direction() const {
  return orientation_ * (-Vector3::kUnitZ);
}

azer::Matrix4 MovableObject::world() const {
  return std::move(Translate(position_)) *
      std::move(orientation_.ToMatrix());
}

void MovableObject::PositionChanged(const Vector3& origin_position) {
  OnPositionChanged(origin_position);
}

void MovableObject::OrientationChanged(const Quaternion& origin_orientation) {
  OnOrientationChanged(origin_orientation);
}
}  // namespace azer
