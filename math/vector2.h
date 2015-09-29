#pragma once

namespace azer {

class Vector2 {
 public:
  Vector2();
  Vector2(float x, float y);
  explicit Vector2(float arr[2]);
  Vector2(const Vector2& v);

  Vector2 operator * (float v) const;
  Vector2 operator + (const Vector2 &v) const;
  Vector2 operator - (const Vector2 &v) const;
  Vector2& operator = (const Vector2& v);

  float dot(const Vector2& v) const;

  float length() const;
  Vector2& Normalize();
  Vector2 NormalizeCopy() const;
 public:
  float x, y;
};

inline Vector2::Vector2() : x(0.0f), y(0.0f) {}
inline Vector2::Vector2(float p1, float p2) : x(p1), y(p2) {}
inline Vector2::Vector2(float arr[2]) : x(*arr), y(*(arr + 1)) {}
inline Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) {}

inline Vector2& Vector2::operator = (const Vector2& v) {
  this->x = v.x;
  this->y = v.y;
  return *this;
}

inline Vector2 Vector2::operator * (float v) const {
  return Vector2(x * v, y * v);
}

inline float Vector2::dot(const Vector2& v) const {
  return x * v.x + y * v.y;
}

inline Vector2 Vector2::operator + (const Vector2 &v) const {
  return Vector2(x + v.x, y + v.y);
}

inline Vector2 Vector2::operator - (const Vector2 &v) const {
  return Vector2(x - v.x, y - v.y);
}

inline float Vector2::length() const {
  return std::sqrt(x * x + y * y);
}

inline Vector2& Vector2::Normalize() {
  float len = length();
  x /= len;
  y /= len;
  return *this;
}

inline Vector2 Vector2::NormalizeCopy() const {
  Vector2 v(x, y);
  v.Normalize();
  return v;
}
}  // namespace azer
