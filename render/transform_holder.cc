#include "azer/render/transform_holder.h"

namespace azer {

TransformHolder::TransformHolder()
    : position_(0.0f, 0.0f, 0.0f),
      scale_(1.0f, 1.0f, 1.0f),
      orientation_(Vector3(0.0, 1.0, 0.0), Degree(0.0f)) {
}


TransformHolder::TransformHolder(const Vector3& position)
    : position_(position),
      scale_(1.0f, 1.0f, 1.0f),
      orientation_(Vector3(0.0, 1.0, 0.0), Degree(0.0f)) {
}

TransformHolder::TransformHolder(const Vector3& pos, const Quaternion& orient)
    : position_(pos),
      scale_(1.0f, 1.0f, 1.0f),
      orientation_(orient) {
}

TransformHolder::TransformHolder(const TransformHolder& obj)
    : position_(obj.position()),
      scale_(1.0f, 1.0f, 1.0f),
      orientation_(obj.orientation_) {
}

TransformHolder& TransformHolder::operator = (const TransformHolder& obj) {
  position_ = obj.position();
  scale_ = obj.scale_;
  orientation_ = obj.orientation_;
  return *this;
}


void TransformHolder::pitch(const Degree angle) {
  pitch(Radians(angle));
}

void TransformHolder::yaw(const Degree angle) {
  yaw(Radians(angle));
}

void TransformHolder::roll(const Degree angle) {
  roll(Radians(angle));
}

void TransformHolder::pitch(const Radians angle) {
  Vector3 axis = orientation_ * Vector3::kUnitX;
  rotate(axis, angle);
}

void TransformHolder::yaw(const Radians angle) {
  Vector3 axis = orientation_ * Vector3::kUnitY;
  rotate(axis, angle);
}

void TransformHolder::roll(const Radians angle) {
  Vector3 axis = orientation_ * Vector3::kUnitZ;
  rotate(axis, angle);
}

void TransformHolder::rotate(const Vector3& axis, const Radians rad) {
  Quaternion q(axis, rad);
  q.Normalize();
  rotate(q);
}

void TransformHolder::rotate(const Vector3& axis, const Degree angle) {
  Quaternion q(axis, angle);
  q.Normalize();
  rotate(q);
}

void TransformHolder::set_orientation(const Quaternion& q) {
  Quaternion origin = orientation_;
  orientation_ = q;
  orientation_.Normalize();
  if (orientation_ != origin) {
    OrientationChanged(origin);
  }
}

void TransformHolder::rotate(const Quaternion& q) {
  DCHECK_FLOAT_EQ(q.length(), 1.0f);
  Quaternion origin = orientation_;
  orientation_ = q * orientation_;
  orientation_.Normalize();
  OrientationChanged(origin);
}

void TransformHolder::walk(float step) {
  Vector3 position = position_;
  position_ +=  directional() * step;
  PositionChanged(position);
}

void TransformHolder::fly(float step) {
  Vector3 position = position_;
  position_ += up() * step;
  PositionChanged(position);
}

void TransformHolder::strafe(float step) {
  Vector3 position = position_;
  position_ += right() * step;
  PositionChanged(position);
}

Vector3 TransformHolder::right() const {
  return orientation_ * Vector3::kUnitX;
}

Vector3 TransformHolder::up() const {
  return orientation_ * Vector3::kUnitY;
}

Vector3 TransformHolder::directional() const {
  return orientation_ * (-Vector3::kUnitZ);
}

Matrix4 TransformHolder::GenWorldMatrix() const {
  return std::move(std::move(Transform::Translate(position_)) *
                   std::move(orientation_.ToMatrix())) *
      std::move(Transform::Scale(scale_));
}

void TransformHolder::PositionChanged(const Vector3& origin_position) {
  OnPositionChanged(origin_position);
}

void TransformHolder::OrientationChanged(const Quaternion& origin_orientation) {
  OnOrientationChanged(origin_orientation);
}
}  // namespace azer
