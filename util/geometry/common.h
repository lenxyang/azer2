#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {
class IndexPack;
class VertexPack;
class VertexDesc;

int32 CalcGeoRoundIndexCount(int32 slice);
int32 CalcGeoRoundVertexCount(int32 slice);
void AppendGeoRoundIndexData(IndexPack* ipack, int slice, Subset* subset);

Subset AppendGeoRoundData(VertexPack* vpack, IndexPack* ipack, float radius, 
                          int slice, const Matrix4& mat = Matrix4::kIdentity);

}  // namespace azer
