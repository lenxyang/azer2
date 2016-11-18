#include "azer/math/math.h"
#include "azer/math/normal_util.h"

namespace azer {

void Plane::reset(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
  point_ = p1;
  normal_ = CalcPlaneNormal(p1, p2, p3);
  DCHECK_LT(std::abs(normal_.length() - 1.0f), 0.0001) << normal_.length();
  d_ = -normal_.dot(p1);
}

float Plane::distance(const Ray& ray) const {
  // 方向如何确定
  // 从 ray 的顶点到平面的方向如何与 ray 的方向一致，则取正
  // 否则取负
  DCHECK(std::abs(normal_.length() - 1.0f) < 0.0001) << normal_.length();
  DCHECK(std::abs(ray.directional().length() - 1.0f) < 0.0001)
      << ray.directional().length();
  float cos_val = normal_.dot(ray.directional());
  if (cos_val != 0.0) {
    Vector3 ddir = point_ - ray.origin();
    float dir = ddir.dot(ray.directional()) >= 0.0f ? 1.0 : -1.0f;
    return dir * std::abs(distance(ray.origin()) / cos_val);
  } else {
    return std::numeric_limits<float>::signaling_NaN();
  }
}

// class triangle
TrianglePlane::TrianglePlane() {
}

void TrianglePlane::reset(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
  Plane::reset(p1, p2, p3);
  points_[0] = p1;
  points_[1] = p2;
  points_[2] = p3;
}

TrianglePlane::TrianglePlane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
    : Plane(p1, p2, p3) {
  points_[0] = p1;
  points_[1] = p2;
  points_[2] = p3;
}

TrianglePlane::TrianglePlane(const Vector3* points)
    : Plane(points) {
  memcpy(points_, points, sizeof(points_));
}

bool TrianglePlane::IsPointIn(const Vector3& pt) const {
  // reference
  // http://www.blackpawn.com/texts/pointinpoly/default.html
  azer::Vector3 v0 = points_[2] - points_[0];  // C - A
  azer::Vector3 v1 = points_[1] - points_[0];  // B - A
  azer::Vector3 v2 = pt - points_[0];
  float d00 = v0.dot(v0);
  float d01 = v0.dot(v1);
  float d02 = v0.dot(v2);
  float d11 = v1.dot(v1);
  float d12 = v1.dot(v2);

  float inv = 1 / (d00 * d11 - d01 * d01);
  float u = (d11 * d02 - d01 * d12) * inv;
  float v = (d00 * d12 - d01 * d02) * inv;
  return (u >= 0.0f) && (v >= 0.0f) && (u + v < 1.0f)
      && belong(pt);
}

std::ostream& operator << (std::ostream& o, const Plane& v) {
  o << "plane[normal: (" << v.normal_.x << ", " << v.normal_.y
    << ", " << v.normal_.z << "), d: " << v.d_ << "]";
  return o;
}

std::ostream& operator << (std::ostream& o, const TrianglePlane& v) {
  o << "plane[normal: " << v.normal_ << ", d: " << v.d_ << "]"
    << "  pt1 " << v.points_[0] << "  pt2 " << v.points_[1]
    << "  pt3 " << v.points_[2];
  return o;
}
}  // namespace azer
