#pragma once

#include <memory>


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/common.h"
#include "azer/math/math.h"

namespace azer {

class Camera;
class Renderer;
class Matrix4;
class Frustum;

enum BoundingVolumnType {
  kNoBoundingVolumn,
  kBoundingSphere,
  kAxisAlignedBoundingBox,
  kOrientedBoundingBox,
};

class AZER_EXPORT BoundingVolumn {
 public:
  BoundingVolumnType type() const { return type_;}
 protected:
  explicit BoundingVolumn(BoundingVolumnType type);
  BoundingVolumnType type_;
};

class AZER_EXPORT AxisAlignedBoundingBox : public BoundingVolumn {
 public:
  AxisAlignedBoundingBox(Vector3 vmin, Vector3 vmax);

  const Vector3& vmin() const { return vmin_;}
  const Vector3& vmax() const { return vmax_;}
  bool IsIntersect(const azer::Ray& ray) const;
 private:
  Vector3 vmin_, vmax_;
  TrianglePlane subplane_[12];
  DISALLOW_COPY_AND_ASSIGN(AxisAlignedBoundingBox);
};

void AZER_EXPORT InitMinAndVMax(Vector3* vmin, Vector3* vmax);
void AZER_EXPORT UpdateVMinAndVMax(const Vector3 pos, Vector3* vmin, Vector3* vmax);
}  // namespace azer
