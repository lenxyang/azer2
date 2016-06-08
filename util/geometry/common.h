#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {
class IndexPack;
class VertexPack;
class VertexDesc;

/**
 * 生成一条三角形组成的带子
 * line1 是上边， line2 是下边, vertex_num 是顶点的数量
 *
 */
void GenTriStripIndex(int32_t line1, int32_t line2, int32_t vertex_num, IndexPack* ipack);

int32_t CalcGeoRoundIndexCount(int32_t slice);
int32_t CalcGeoRoundVertexCount(int32_t slice);

/**
 * 生成一个顶部向上（向下的） Taper 的 index 数据  
 * 注：圆形是一个特殊的 Taper
 *
 */
int32_t AppendUpGeoTaperIndexData(int32_t base, IndexPack* ipack, int slice);
int32_t AppendBottomGeoTaperIndexData(int32_t last, IndexPack* ipack, int slice);

Subset AppendGeoRoundData(VertexPack* vpack, IndexPack* ipack, float radius, 
                          int slice, const Matrix4& mat = Matrix4::kIdentity);

void AppendGeoRoundSubset(EntityData* data, float radius, int slice, 
                          const Matrix4& mat = Matrix4::kIdentity);

// Circle
Subset AppendGeoCircleData(VertexPack* vpack, float radius, int slice,
                           const Matrix4& mat = Matrix4::kIdentity);
void AppendGeoCircleSubset(EntityData* data, float radius, int slice, 
                           const Matrix4& mat = Matrix4::kIdentity);

void UpdateVertexBounds(const Vector4 pos, Vector3* vmin, Vector3* vmax);
}  // namespace azer
