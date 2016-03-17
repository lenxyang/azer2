#include "azer/util/geometry/sphere.h"

#include "azer/render/index_pack.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/vertex_buffer.h"
#include "azer/util/geometry/common.h"

namespace azer {
inline int32 CalcSphereIndexCount(int32 stack_num, int32 slice_num) {
  return (stack_num - 2 - 1) * slice_num * 3 * 2 + slice_num * 2 * 3;
}

inline int32 CalcSphereVertexCount(int32 stack_num, int32 slice_num) {
  return (stack_num - 2) * (slice_num + 1) + 2;
}

Subset AppendGeoSphereSuset(VertexPack* vp, IndexPack* ipack,
                            const GeoSphereParam& p, const Matrix4& mat) {
  VertexPos vpos(0, 0), texpos;
  GetSemanticIndex("texcoord", 0, vp->desc(), &texpos);

  Subset subset;
  subset.vertex_base = vp->index();
  vp->WriteVector3Or4(mat * Vector4(0.0f, p.radius, 0.0f, 1.0f), vpos);
  vp->WriteVector2(Vector2(0.0f, 0.0f), texpos);
  CHECK(vp->next(1));
  for (int i = 1; i < p.stack - 1; ++i) {
    float y = p.radius * sin(Degree(90.0f - i * 180.0f / (float)p.stack));
    float slice_radius =  cos(Degree(90.0f - i * 180.0f / (float)p.stack));
    float tv = i * 1.0f / (p.stack - 1);
    for (int j = 0; j < p.slice; ++j) {
      float degree = 360.0f - j * 360.0f / p.slice;
      float x = p.radius * slice_radius * cos(Degree(degree));
      float z = p.radius * slice_radius * sin(Degree(degree));
      vp->WriteVector3Or4(Vector4(x, y, z, 1.0f), vpos);
      float tu = j * 1.0f / p.slice;
      vp->WriteVector2(Vector2(tu, tv), texpos);
      CHECK(vp->next(1));
    }
    Vector4 pos = std::move(mat * Vector4(slice_radius, y, 0.0f, 1.0f));
    vp->WriteVector3Or4(pos, vpos);
    vp->WriteVector2(Vector2(1.0f, tv), texpos);
    CHECK(vp->next(1));
  }

  Vector4 pos = std::move(mat * Vector4(0.0F, -p.radius, 0.0f, 1.0f));
  vp->WriteVector3Or4(pos, vpos);
  vp->WriteVector2(Vector2(1.0f, 1.0f), texpos);
  vp->next(1);
  subset.vertex_count = vp->index() - subset.vertex_base;

  subset.index_base = ipack->index();
  AppendUpGeoTaperIndexData(subset.index_base, ipack, p.slice);
  for (int i = 1; i < p.stack - 2; ++i) {
    int line1 = subset.index_base + 1 + p.slice * (i - 1);
    int line2 = subset.index_base + 1 + p.slice * i;
    GenTriStripIndex(line1, line2, p.slice, ipack);
  }
  AppendBottomGeoTaperIndexData(subset.index_base, ipack, p.slice);
  subset.index_count = ipack->index() - subset.index_base;

  return subset;
}

void AppendGeoSphereData(EntityData* data, const GeoSphereParam& p, 
                         const Matrix4& mat) {
  const int32 kVertexCount = CalcSphereVertexCount(p.stack, p.slice);
  const int32 kIndexCount = CalcSphereIndexCount(p.stack, p.slice);
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  vpack.move(data->vdata()->vertex_count() - 1 - kVertexCount);
  ipack.move(data->idata()->count() - 1 - kIndexCount);
  Subset subset = AppendGeoSphereSuset(&vpack, &ipack, p, mat);
  data->AddSubset(subset);
}

EntityDataPtr CreateSphere(VertexDesc* desc, const GeoSphereParam& p,
                           const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata, idata));
  AppendGeoSphereData(data.get(), p, mat);
  return data;
}
}  // namespace azer
