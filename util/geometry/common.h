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
void GenTriStripIndex(int32 line1, int32 line2, int32 vertex_num, IndexPack* ipack);

int32 CalcGeoRoundIndexCount(int32 slice);
int32 CalcGeoRoundVertexCount(int32 slice);

/**
 * 生成一个顶部向上（向下的） Taper 的 index 数据  
 * 注：圆形是一个特殊的 Taper
 *
 */
int32 AppendUpGeoTaperIndexData(int32 base, IndexPack* ipack, int slice);
int32 AppendBottomGeoTaperIndexData(int32 base, IndexPack* ipack, int slice);

Subset AppendGeoRoundData(VertexPack* vpack, IndexPack* ipack, float radius, 
                          int slice, const Matrix4& mat = Matrix4::kIdentity);


void UpdateVertexBounds(const Vector4 pos, Vector3* vmin, Vector3* vmax);
}  // namespace azer
