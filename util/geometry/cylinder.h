#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {

class IndexPack;
class VertexPack;
class VertexDesc;

struct GeoCylinderParam {
  GeoCylinderParam() 
      : top_radius(1.0f), bottom_radius(1.0f), height(1.0f), 
        slice(64), stack(5) {}
  float top_radius;
  float bottom_radius;
  float height;
  int32_t slice;
  int32_t stack;
};

Subset AppendGeoBarrelData(VertexPack* vp, IndexPack* ipack, 
                           const GeoCylinderParam& p, 
                           const Matrix4& mat = Matrix4::kIdentity);

void AppendGeoBarrelSubset(EntityData* data, const GeoCylinderParam& p, 
                          const Matrix4& mat = Matrix4::kIdentity);

void AppendGeoCylinderSubset(EntityData* data, const GeoCylinderParam& p, 
                           const Matrix4& mat);
EntityDataPtr CreateCylinder(VertexDesc* desc, const GeoCylinderParam& p,
                             const Matrix4& mat = Matrix4::kIdentity);

}  // namespace azer
