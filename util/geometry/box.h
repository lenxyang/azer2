#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {
class IndexPack;
class VertexPack;
class VertexDesc;

// position vector
// near_left_top, near_right_top,
// near_right_bottom, near_left_bottom,
// far_left_top, far_right_top,
// far_right_bottom, far_left_bottom,
int* GetHexaHedronTriangleListIndex();
int* GetHexaHedronFramelineIndex();
Subset AppendGeoHexaHedronSubset(VertexPack* vp, const Vector4* posvec,
                                 const Matrix4& mat = Matrix4::kIdentity);

Subset AppendGeoBoxSubset(VertexPack* vp, IndexPack* ipack, 
                          const Matrix4& mat = Matrix4::kIdentity);

void AppendGeoHexaHedronData(EntityData* data, const Vector4* posvec, 
                             const Matrix4& mat);
void AppendGeoHexaHedronFrameData(EntityData* data, const Vector4* posvec, 
                                  const Matrix4& mat);

void AppendGeoBoxData(EntityData* data, const Matrix4& mat);
void AppendGeoBoxFrameData(EntityData* data, const Matrix4& mat);
EntityDataPtr CreateBox(VertexDesc* desc, const Matrix4& m = Matrix4::kIdentity);
EntityDataPtr CreateBoxFrame(VertexDesc* d, const Matrix4& m = Matrix4::kIdentity);


// 按道理是不应该出现使用 Index Buffer 的 Box，因为 Box 的公共定点的发现无法共享
// 下面函数生成的 Box 仅仅用于没有发现的情况
// 如果 VertexDesc 包含发现，将被填为 Vector4(0, 0, 0, 0);
Subset AppendGeoHexaHedronIndexedSubset(VertexPack* vpack, IndexPack* ipack,
                                        const Vector4* posvec, const Matrix4& mat);
void AppendGeoHexaHedronIndexedData(EntityData* data, const Vector4* vecpos, 
                                    const Matrix4& mat);
void AppendGeoBoxIndexedData(EntityData* data, const Matrix4& mat);
EntityDataPtr CreateBoxIndexed(VertexDesc* desc, const Matrix4& m = Matrix4::kIdentity);

// 创建任意六面体
EntityDataPtr CreateHexaHedron(VertexDesc* d, const Vector4* vecpos,
                               const Matrix4& m);
EntityDataPtr CreateHexaHedronFrame(VertexDesc* d, const Vector4* vecpos,
                                    const Matrix4& m);
}  // namespace azer
