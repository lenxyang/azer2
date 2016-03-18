#pragma once

#include "azer/math/matrix4.h"
#include "azer/render/entity.h"

namespace azer {
class IndexPack;
class VertexPack;

struct GeoSphereParam {
  GeoSphereParam() : radius(1.0f), stack(64), slice(64) {}
  float radius;
  int32 stack;
  int32 slice;
};

Subset AppendGeoSphereSuset(VertexPack* vp, IndexPack* ipack,
                            const GeoSphereParam& p, 
                            const Matrix4& mat = Matrix4::kIdentity);
Subset AppendGeoSphereFrameSuset(VertexPack* vp, IndexPack* ipack,
                                 const GeoSphereParam& p, 
                                 const Matrix4& mat = Matrix4::kIdentity);
void AppendGeoSphereData(EntityData* data, const GeoSphereParam& p, 
                         const Matrix4& mat);
void AppendGeoSphereFrameData(EntityData* data, const GeoSphereParam& p, 
                              const Matrix4& mat);
EntityDataPtr CreateSphere(VertexDesc* desc, const GeoSphereParam& p,
                           const Matrix4& mat = Matrix4::kIdentity);
EntityDataPtr CreateSphereFrame(VertexDesc* desc, const GeoSphereParam& p,
                                const Matrix4& mat = Matrix4::kIdentity);
}  // namespace azer
