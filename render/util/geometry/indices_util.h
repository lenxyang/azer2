#pragma once

#include "base/basictypes.h"
#include "azer/render/util/index_pack.h"

namespace azer {
bool GenerateTopTriangleStrip(int32 top, int32 begin, int32 vertex_num,
                              bool closed, IndexPack* pack);
bool GenerateBottomTriangleStrip(int32 bottom, int32 begin, int32 vertex_num,
                                 bool closed, IndexPack* pack);
bool GenerateStripIndex(int32 line1, int32 line2, int32 vertex_num, bool closed,
                          IndexPack* ipack);

int32 GetSemanticIndex(const std::string& name, int32 semantic_index, 
                       VertexDesc* desc);


void CalcNormal(VertexData* vbd, IndicesData* idata);
void CalcTriangleListNormal(VertexData* vbd, int* indices);
}  // namespace azer


