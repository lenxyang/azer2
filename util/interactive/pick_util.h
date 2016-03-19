#pragma once

#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/size.h"
#include "azer/math/ray.h"
#include "azer/math/vector3.h"
#include "azer/math/vector4.h"

namespace azer {
class Plane;
class Camera;

Vector4 CalcWorldPosFromScreen(const gfx::Point& pt, const gfx::Size& size, 
                               const Camera* camera);

/**
 * Fucntion GetPickingRay
 * 由屏幕坐标点及 Camera 信息获得一条射线
 * params: pt 屏幕坐标点
 *
 */
Ray GetPickingRay(const gfx::Point& pt, const gfx::Size& size,
                  const Camera* camera);

/**
 * 获得射线与平面的交点
 * 如果有交点返回 true, 如果没有交点（平行）返回 false
 *
 */
bool PickingPlane(const Ray& ray, const Plane& plane, Vector3* pt);

/**
 * 计算点在 NDC 空间内的位置
 * 
 */
void CalcNCDVec(const Vector3& ps, const Camera& camera, Vector3* ndc);

namespace detail {
float CalcDepthValue(const Vector3& ps, const Camera& camera);
}
}  // namespace azer
