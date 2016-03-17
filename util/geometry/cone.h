#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {
class IndexPack;
class VertexPack;
class VertexDesc;

struct GeoConeParam {
  GeoConeParam() : height(1.0f), radius(1.0f), slice(64) {}
  float height;
  float radius;
  int slice;
};


Subset AppendGeoTaperData(VertexPack* vp, IndexPack* ipack, const GeoConeParam& p, 
                          const Matrix4& mat = Matrix4::kIdentity);

void AppendGeoConeData(EntityData* data, const GeoConeParam& p, const Matrix4& mat);
EntityDataPtr CreateCone(VertexDesc* desc, const GeoConeParam& p,
                         const Matrix4& mat = Matrix4::kIdentity);
}  // namespace azer
