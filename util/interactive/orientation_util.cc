#include "azer/util/interactive/orientation_util.h"

#include "azer/math/vector3.h"
#include "azer/math/quaternion.h"
#include "azer/render/camera.h"

namespace azer {
void CalcSceneOrientForZDirection(const Vector3& d, Quaternion* orient) {
  Camera camera;
  Vector3 dir = d.NormalizeCopy();
  Vector3 up = Vector3(dir.x, 10.0f, dir.z);
  Vector3 newz = dir.NormalizeCopy();
  Vector3 newx = std::move(up.cross(newz).NormalizeCopy());
  Vector3 newy = std::move(newz.cross(newx).NormalizeCopy());
  *orient = Quaternion::FromAxis(newx, newy, newz);
  Vector3 newzz = orient->zaxis();
}

}  // namespace azer
