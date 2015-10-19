#include "azer/render/axis_aligned_box.h"

#include <limits>

#include "azer/render/render_system.h"
#include "azer/render/frustrum.h"
#include "azer/render/camera.h"
#include "base/logging.h"

namespace azer {

AxisAlignedBox AxisAlignedBox::CalcForVertexData(
    const SlotVertexData& vdata) {
  azer::Vector4 min_pos(std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max(), 1.0f);
  azer::Vector4 max_pos(std::numeric_limits<float>::min(),
                        std::numeric_limits<float>::min(),
                        std::numeric_limits<float>::min(), 1.0f);
  DataFormat pos_type = vdata.desc()->descs()[0].type;
  DCHECK(pos_type == kVec3 || pos_type == kVec4);
  const uint8* next = vdata.pointer();
  CHECK(next != NULL);
  do {
    if (pos_type == kVec3) {
      Vector3* pos = (Vector3*)next;
      min_pos.x = std::min(min_pos.x, pos->x);
      min_pos.y = std::min(min_pos.y, pos->y);
      min_pos.z = std::min(min_pos.z, pos->z);

      max_pos.x = std::max(max_pos.x, pos->x);
      max_pos.y = std::max(max_pos.y, pos->y);
      max_pos.z = std::max(max_pos.z, pos->z);
    } else if (pos_type == kVec4) {
      Vector4* pos = (Vector4*)next;
      min_pos.x = std::min(min_pos.x, pos->x);
      min_pos.y = std::min(min_pos.y, pos->y);
      min_pos.z = std::min(min_pos.z, pos->z);
      
      max_pos.x = std::max(max_pos.x, pos->x);
      max_pos.y = std::max(max_pos.y, pos->y);
      max_pos.z = std::max(max_pos.z, pos->z);
    } else {
      CHECK(false) << "Type not support: " << pos_type;
    }
  } while ((next = vdata.next(next)));

  return AxisAlignedBox(min_pos, max_pos);
}

AxisAlignedBox AxisAlignedBox::TransformCopy(const Matrix4& trans) {
  AxisAlignedBox box;
  box.minimum_ = trans * minimum_;
  box.maximum_ = trans * maximum_;
  return box;
}

bool AxisAlignedBox::intersect(const Ray& ray) const {
  // front
  // just test a plane inside the cube
  TrianglePlane plane1(minimum_, maximum_, 
                       vec3(maximum_.x, minimum_.y, maximum_.z));
  TrianglePlane plane2(minimum_, maximum_, 
                       vec3(minimum_.x, maximum_.y, minimum_.z));

  vec3 pos = plane1.intersect(ray);
  // LOG(ERROR) << "interact: " << pos << ", dist: " << plane1.distance(ray);
  // LOG(ERROR) << *this;
  return this->inside(pos);
}

namespace {
inline VisibleState IsAABBVisible(const AxisAlignedBox& aabb,
                                  const Frustrum& frustrum) {
  Vector3 center((aabb.maximum() + aabb.minimum()) / 2.0f);
  Vector3 halfsize((aabb.maximum() - aabb.minimum()) / 2.0f);
  return frustrum.IsVisible(center, halfsize);
}
}  // namespace

AxisAlignedBox::AxisAlignedBox()
    : is_null_(true)  {
}

AxisAlignedBox::AxisAlignedBox(
    const AxisAlignedBox& box) {
  *this = box;
}

AxisAlignedBox::AxisAlignedBox(const Vector3& minimum, const Vector3& maximum)
    : minimum_(minimum, 1.0f)
    , maximum_(maximum, 1.0f)
    , is_null_(false) {
  DCHECK(minimum_.x <= maximum_.x &&
         minimum_.y <= maximum_.y &&
         minimum_.z <= maximum_.z);
}

AxisAlignedBox::AxisAlignedBox(const Vector4& minimum, const Vector4& maximum)
    : minimum_(minimum)
    , maximum_(maximum)
    , is_null_(false) {
  DCHECK(minimum_.x <= maximum_.x &&
         minimum_.y <= maximum_.y &&
         minimum_.z <= maximum_.z);
}

AxisAlignedBox& AxisAlignedBox::operator = (
    const AxisAlignedBox& box) {
  this->minimum_ = box.minimum_;
  this->maximum_ = box.maximum_;
  this->is_null_ = box.is_null_;
  return *this;
}

void AxisAlignedBox::merge(const AxisAlignedBox& aabbox) {
  minimum_.x = std::min(aabbox.minimum().x, minimum_.x);
  minimum_.y = std::min(aabbox.minimum().y, minimum_.y);
  minimum_.z = std::min(aabbox.minimum().z, minimum_.z);

  maximum_.x = std::max(aabbox.maximum().x, maximum_.x);
  maximum_.y = std::max(aabbox.maximum().y, maximum_.y);
  maximum_.z = std::max(aabbox.maximum().z, maximum_.z);
  is_null_ = false;
}

bool AxisAlignedBox::inside(const Vector3& pos) const {
  return minimum_.x <= pos.x && maximum_.x >= pos.x  &&
      minimum_.y <= pos.y && maximum_.y >= pos.y  &&
      minimum_.z <= pos.z && maximum_.z >= pos.z;
}

bool AxisAlignedBox::intersect(const AxisAlignedBox& box)
    const {
  return inside(box.minimum_) || inside(box.maximum_);
}

VisibleState AxisAlignedBox::IsVisible(const Frustrum& frustrum) {
  return IsAABBVisible(*this, frustrum);
}

std::ostream& operator << (std::ostream& o, const AxisAlignedBox& v) {
  o << "AxisAlignedBox["
    << " minimum: (" << v.minimum_.x << ", " << v.minimum_.y << ", "
    << v.minimum_.z << ") "
    << ", maximum: (" << v.maximum_.x << ", " << v.maximum_.y << ", " << v.maximum_.z << ") ]";
  return o;
}
}  // namespace azer
