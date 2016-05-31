#include "azer/math/math.h"
#include "base/logging.h"

namespace azer {
Vector3 CalcTangent(const Vector3& p1, const Vector2& tex1,
                    const Vector3& p2, const Vector2& tex2,
                    const Vector3& p3, const Vector2& tex3) {
  azer::Vector3 n = std::move(CalcPlaneNormal(p1, p2, p3));
  Vector3 v1 = p2 - p1;
  Vector3 v2 = p3 - p1;
  Vector2 dt1 = std::move(tex2 - tex1);
  Vector2 dt2 = std::move(tex3 - tex1);
  float cons = (1.0f / (dt1.x * dt2.y - dt2.x * dt1.y));
  float x = (dt1.y * v1.x - dt2.y * v2.x) * cons;
  float y = (dt1.y * v1.y - dt2.y * v2.y) * cons;
  float z = (dt1.y * v1.z - dt2.y * v2.z) * cons;
  return Vector3(x, y, z);
}

void CalcTriangleTBN(const Vector3& p1, const Vector2& tex1,
                     const Vector3& p2, const Vector2& tex2,
                     const Vector3& p3, const Vector2& tex3,
                     Vector3* tangent, Vector3* normal, Vector3* binormal) {
  Vector3 t, b;
  azer::Vector3 n = std::move(CalcPlaneNormal(p1, p2, p3));
  n.Normalize();
  Vector3 v1 = p2 - p1;
  Vector3 v2 = p3 - p1;
  Vector2 dt1 = std::move(tex2 - tex1);
  Vector2 dt2 = std::move(tex3 - tex1);
  float cons = (1.0f / (dt1.x * dt2.y - dt2.x * dt1.y));
  t.x = (dt2.y * v1.x - dt1.y * v2.x) * cons;
  t.y = (dt2.y * v1.y - dt1.y * v2.y) * cons;
  t.z = (dt2.y * v1.z - dt1.y * v2.z) * cons;

  b.x = (dt1.x * v2.x - dt2.x * v1.x) * cons;
  b.y = (dt1.x * v2.y - dt2.x * v1.y) * cons;
  b.z = (dt1.x * v2.z - dt2.x * v1.z) * cons;
  
  t = (t - n* (t.dot(n)));
  b = t.cross(n);
  *tangent = t.NormalizeCopy();
  *normal = n.NormalizeCopy();
  *binormal = b.NormalizeCopy();
}

Vector3 CalcPlaneNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
  Vector3 v1 = p2 - p1;
  Vector3 v2 = p3 - p1;
  return CrossProduct(v1, v2).Normalize();
}

Vector3 CalcPlaneNormal(const Vector4& p1, const Vector4& p2, const Vector4& p3) {
  Vector4 v1 = p2 - p1;
  Vector4 v2 = p3 - p1;
  return CrossProduct(Vector3(v1), Vector3(v2)).Normalize();
}
}  // namespace azer
