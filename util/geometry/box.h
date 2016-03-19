#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {
class IndexPack;
class VertexPack;
class VertexDesc;

Subset AppendGeoBoxSubset(VertexPack* vp, IndexPack* ipack, 
                          const Matrix4& mat = Matrix4::kIdentity);

void AppendGeoBoxData(EntityData* data, const Matrix4& mat);
void AppendGeoBoxFrameData(EntityData* data, const Matrix4& mat);
EntityDataPtr CreateBox(VertexDesc* desc, const Matrix4& m = Matrix4::kIdentity);
EntityDataPtr CreateBoxFrame(VertexDesc* d, const Matrix4& m = Matrix4::kIdentity);
}  // namespace azer
