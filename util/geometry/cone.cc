#include "azer/util/geometry/cone.h"

#include "azer/render/index_pack.h"
#include "azer/render/vertex_pack.h"
#include "azer/util/geometry/common.h"
#include "azer/util/geometry/normal_util.h"

namespace azer {
Subset AppendGeoTaperData(VertexPack* vpack, IndexPack* ipack, const GeoConeParam& p,
                          const Matrix4& mat) {
  const int32 kVertexNum = 1 + p.slice + 1;
  Subset subset;
  VertexPos vpos(0, 0), npos;
  GetSemanticIndex("normal", 0, vpack->desc(), &npos);

  float degree = 360.0f / (float)p.slice;
  vpack->WriteVector3Or4(mat * Vector4(0, p.height, 0, 1.0f), vpos);
  vpack->next(1);
  for (int i = 1; i < kVertexNum; ++i) {
    float x = cos(Degree(i * degree)) * p.radius;
    float z = sin(Degree(i * degree)) * p.radius;
    vpack->WriteVector3Or4(mat * Vector4(x, 0, z, 1.0f), vpos);
    vpack->next(1);
  }
  subset.vertex_count = vpack->index() - subset.vertex_base;

  AppendGeoRoundIndexData(ipack, p.slice, &subset);
  CalcIndexedTriangleNormal(vpack->data(), ipack->data(), subset);
  CalcIndexedTriangleListTangentAndBinormal(vpack->data(), ipack->data(), subset);
  return subset;
}

void AppendGeoConeData(EntityData* data, const GeoConeParam& p, const Matrix4& mat){
  Subset sub;
  const int32 kVertexCount = CalcGeoRoundVertexCount(p.slice);
  const int32 kIndexCount = CalcGeoRoundIndexCount(p.slice);
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  vpack.move(data->vdata()->vertex_count() - kVertexCount - 1);
  ipack.move(data->idata()->count() - kIndexCount - 1);
  sub = AppendGeoRoundData(&vpack, &ipack, p.radius, p.slice, mat);
  data->AddSubset(sub);

  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  sub = AppendGeoTaperData(&vpack, &ipack, p, mat);
  data->AddSubset(sub);
}

EntityDataPtr CreateCone(VertexDesc* desc, const GeoConeParam& p,                         const Matrix4& mat) {
  EntityDataPtr data(new EntityData(desc, 1));
  AppendGeoConeData(data.get(), p, mat);
  return data;
}
}  // namespace 
