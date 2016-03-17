#pragma once

#include "azer/base/export.h"
#include "azer/render/entity.h"

namespace azer {
class VertexData;
class IndicesData;

void CalcIndexedTriangleNormal(VertexData* vbd, IndicesData* idata, 
                               const Subset& subset);
void CalcIndexedTriangleListTangentAndBinormal(VertexData* vd, IndicesData* id, 
                                               const Subset& subset);
}  // namespace azer
