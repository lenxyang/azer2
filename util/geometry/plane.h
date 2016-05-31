#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {
class IndexPack;
class VertexPack;
class VertexDesc;

struct PlaneParam {
  float width;
  float depth;
  int column;
  int row;
};

Subset AppendGeoPlaneData(VertexPack* vpack, IndexPack* ipack, 
                          const PlaneParam& param,
                          const Matrix4& mat = Matrix4::kIdentity);
void AppendGeoPlaneSubset(EntityData* data, const PlaneParam& param,
                        const Matrix4& mat = Matrix4::kIdentity);
EntityDataPtr CreatePlane(VertexDesc* desc, const PlaneParam& param,
                          const Matrix4& mat = Matrix4::kIdentity);

Subset AppendGeoSquareData(VertexPack* vpack, IndexPack* ipack, 
                           float length, int32_t slice,
                           const Matrix4& mat = Matrix4::kIdentity);
void AppendGeoSquareSubset(EntityData* data, float length, int32_t slice,
                           const Matrix4& mat = Matrix4::kIdentity);
EntityDataPtr CreateSquare(VertexDesc* desc, float length, int32_t slice,
                           const Matrix4& mat = Matrix4::kIdentity);
}  // namespace azer
