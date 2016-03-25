#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {
class IndexPack;
class VertexPack;
class VertexDesc;

int32 AppendGeoPlaneSubset(EntityData* data, const PlaneParam& param,
                           const Matrix4& mat = Matrix4::kIdentity);
EntityDataPtr CreateArrow(VertexDesc* desc, const PlaneParam& param,
                          const Matrix4& mat = Matrix4::kIdentity);
}  // namespace azer
