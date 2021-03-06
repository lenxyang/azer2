#include "azer/util/interactive/pick_util.h"

#include "azer/math/math.h"
#include "azer/render/camera.h"

namespace azer {
Vector4 CalcWorldPosFromScreen(const gfx::Point& pt, const gfx::Size& size, 
                               const Camera* camera) {
  float x = 2 * (float)pt.x() / (float)size.width() - 1.0f;
  float y = -2 * (float)pt.y() / (float)size.height() + 1.0f;
  Vector4 pos(x, y, 0.0f, 1.0f);

  const Matrix4& proj = camera->frustum().projection();
  const Matrix4& view = camera->GetViewMatrix();
  Matrix4 mat = std::move(view.InverseCopy() * proj.InverseCopy());
  
  pos = mat * pos;
  pos /= pos.w;
  return pos;
}

Ray GetPickingRay(const gfx::Point& pt, const gfx::Size& size,
                  const Camera* camera) {
  // another method
  // reference 3D Game engine design(2nd edition) 8.4, Object Picking
  Vector4 pos = Vector4(camera->holder().position(), 1.0f);
  Vector4 pos2 = CalcWorldPosFromScreen(pt, size, camera);
  Vector4 dir = pos - pos2;
  Vector3 nd(dir.x, dir.y, dir.z);
  nd.Normalize(); 
  return Ray(Vector3(pos.x, pos.y, pos.z), nd);
}

bool PickingPlane(const azer::Ray& ray, const azer::Plane& plane, 
                  azer::Vector3* pt) {
  float dot = ray.directional().dot(plane.normal());
  if (std::abs(dot) < 0.001) {
    return false;
  }

  *pt = plane.intersect(ray);
  return true;
}

void CalcNCDVec(const Vector3& p, const Camera& camera, Vector3* ndc) {
  Vector4 v = camera.GetProjViewMatrix() * Vector4(p, 1.0f);
  ndc->x = v.x / v.w;
  ndc->y = v.y / v.w;
  ndc->z = v.z / v.w;
}

namespace detail {
float CalcDepthValue(const Vector3& p, const Camera& camera) {
  Vector3 d;
  CalcNCDVec(p, camera, &d);
  return d.z;
}
}  // namespace
}  // namespace azer
