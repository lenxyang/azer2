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
Subset AppendGeoHexaHedronSubset(VertexPack* vp, const Vector4* posvec,
                                 const Matrix4& mat = Matrix4::kIdentity);

Subset AppendGeoBoxSubset(VertexPack* vp, IndexPack* ipack, 
                          const Matrix4& mat = Matrix4::kIdentity);

void AppendGeoBoxData(EntityData* data, const Matrix4& mat);
void AppendGeoHexaHedronData(EntityData* data, const Vector4* posvec, 
                             const Matrix4& mat);
void AppendGeoBoxFrameData(EntityData* data, const Matrix4& mat);
EntityDataPtr CreateBox(VertexDesc* desc, const Matrix4& m = Matrix4::kIdentity);
EntityDataPtr CreateBoxFrame(VertexDesc* d, const Matrix4& m = Matrix4::kIdentity);

EntityDataPtr CreateHexaHedron(VertexDesc* d, const Vector4* vecpos,
                               const Matrix4& m);
EntityDataPtr CreateHexaHedronFrame(VertexDesc* d, const Vector4* vecpos,
                                    const Matrix4& m);
}  // namespace azer
