#pragma once

#include <cmath>
#include <algorithm>
#include "base/basictypes.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/render/bounding_volumn.h"

namespace azer {

class Camera;
class RenderSystem;
class SlotVertexData;

class AZER_EXPORT AxisAlignedBox : public BoundingVolumn {
 public:
  AxisAlignedBox();
  AxisAlignedBox(const Vector4& minimum, const Vector4& maximum);
  AxisAlignedBox(const Vector3& minimum, const Vector3& maximum);
  AxisAlignedBox(const AxisAlignedBox& box);
  static AxisAlignedBox CalcForVertexData(const SlotVertexData& vdata);

  AxisAlignedBox& operator = (const AxisAlignedBox& box);

  /**
   * 在物体变换过程中（包括缩放，位移，旋转等）
   * 想要使得 AABB 能够继续包围物体必须同时对 AABB 进行变换
   */
  AxisAlignedBox TransformCopy(const Matrix4& trans);
  void merge(const AxisAlignedBox& aabox);

  const Vector4& minimum() const { return minimum_;}
  const Vector4& maximum() const { return maximum_;};
  void set_min(const Vector4& m) { minimum_ = m;}
  void set_max(const Vector4& m) { maximum_ = m;}

  bool intersect(const Ray& ray) const;
  bool intersect(const AxisAlignedBox& box) const;
  bool inside(const Vector3& pos) const;
  bool IsNull() const { return is_null_;}

  // from Bounding Volumn
  VisibleState IsVisible(const Frustrum& frustrum) override;
  friend AZER_EXPORT std::ostream& operator << (std::ostream& o,
                                                const AxisAlignedBox& v);
 private:
  Vector4 minimum_;
  Vector4 maximum_;
  bool is_null_;
};
}  // namespace azer
