#pragma once

#include "base/basictypes.h"
#include "azer/render/util/index_pack.h"
#include "azer/render/util/vertex_pack.h"

namespace azer {
bool GenerateTopTriangleStrip(int32 top, int32 begin, int32 vertex_num,
                              bool closed, IndexPack* pack);
bool GenerateBottomTriangleStrip(int32 bottom, int32 begin, int32 vertex_num,
                                 bool closed, IndexPack* pack);
bool GenerateStripIndex(int32 line1, int32 line2, int32 vertex_num, bool closed,
                          IndexPack* ipack);

int32 GetSemanticIndex(const std::string& name, int32 semantic_index, 
                       VertexDesc* desc);
int32 GetSemanticIndex(const std::string& name, int32 semantic_index, 
                       const std::vector<VertexDesc::Desc>& desc);


void CalcNormal(SlotVertexData* vbd, IndicesData* idata);
void CalcTriangleListNormal(SlotVertexData* vbd, int* indices);

void GenerateConeHat(bool up, float top, float bottom, int32 slice, 
                     VertexPack* vpack, IndexPack* ipack);

void GenerateBarrel(float top_radius, float bottom_radius, float height, 
                    int32 stack, int32 slice, VertexPack* vpack, IndexPack* ipack);
}  // namespace azer


