#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"

namespace azer {

void WriteRoundData(Subset* subset, VertexPack* vpack, IndexPack* ipack, 
                    float radius, int32 slice, const Matrix4& mat);
}  // namespace azer
