#pragma once

#include "base/basictypes.h"
#include "azer/render/index_pack.h"
#include "azer/render/vertex_pack.h"

namespace azer {
void AZER_EXPORT CalcNormal(SlotVertexData* vbd, IndicesData* idata);
void AZER_EXPORT CalcTriangleListNormal(SlotVertexData* vbd, int* indices);
}  // namespace azer
