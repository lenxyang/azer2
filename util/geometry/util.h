#pragma once

#include "azer/math/math.h"
#include "azer/render/common.h"
#include "azer/render/entity.h"


namespace azer {
EntityPtr CreateGeoPointsList(PrimitiveTopology primitive,
                              const Vector3* points, int count,
                              VertexDesc* desc, const Matrix4& mat);

// util
void CalcIndexedTriangleNormal(VertexData* vbd, IndicesData* idata);
void CalcTriangleNormal(VertexData* vbd, int* indices);
void CalcIndexedTriangleListTangentAndBinormal(VertexData* vd, IndicesData* id);
}  // namespace azer
