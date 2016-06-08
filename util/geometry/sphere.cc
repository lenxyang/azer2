#include "azer/util/geometry/sphere.h"

#include "azer/render/index_pack.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/vertex_buffer.h"
#include "azer/util/geometry/common.h"
#include "azer/util/geometry/normal_util.h"

namespace azer {
namespace {
inline int32_t CalcSphereIndexCount(int32_t stack_num, int32_t slice_num) {
  return (stack_num - 2 - 1) * slice_num * 3 * 2 + slice_num * 2 * 3;
}

inline int32_t CalcSphereFrameIndexCount(int32_t stack, int32_t slice) {
  return (stack - 1) * (slice + 1) * 2 + (stack - 2) * (slice + 1) * 2;
}

inline int32_t CalcSphereVertexCount(int32_t stack_num, int32_t slice_num) {
  return (stack_num - 2) * (slice_num + 1) + 2;
}

Subset AppendGeoSphereVertexSuset(VertexPack* vp, IndexPack* ipack,
                                  const GeoSphereParam& p, const Matrix4& mat) {
  VertexPos vpos(0, 0), texpos;
  GetSemanticIndex("texcoord", 0, vp->desc(), &texpos);

  Subset subset;
  subset.vertex_base = vp->index();
  Vector4 pos = mat * Vector4(0.0f, p.radius, 0.0f, 1.0f);
  float degree_unit = 360.0f / (float)p.slice;
  vp->WriteVector3Or4(pos, vpos);
  UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
  vp->WriteVector2(Vector2(0.0f, 0.0f), texpos);
  CHECK(vp->next(1));
  for (int i = 1; i < p.stack - 1; ++i) {
    float y = p.radius * sin(Degree(90.0f - i * 180.0f / (float)(p.stack - 1)));
    float slice_radius =  cos(Degree(90.0f - i * 180.0f / (float)(p.stack - 1)));
    float tv = i * 1.0f / (p.stack - 1);
    for (int j = 0; j < p.slice + 1; ++j) {
      float degree = degree_unit * j;
      float x = p.radius * slice_radius * cos(Degree(degree));
      float z = p.radius * slice_radius * sin(Degree(degree));
      float tu = j * 1.0f / p.slice;
      Vector4 pos = mat * Vector4(x, y, z, 1.0f);
      vp->WriteVector3Or4(pos, vpos);
      UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
      vp->WriteVector2(Vector2(tu, tv), texpos);
      CHECK(vp->next(1));
    }
  }

  pos = std::move(mat * Vector4(0.0f, -p.radius, 0.0f, 1.0f));
  vp->WriteVector3Or4(pos, vpos);
  UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
  vp->WriteVector2(Vector2(1.0f, 1.0f), texpos);
  vp->next(1);

  subset.vertex_count = vp->index() - subset.vertex_base;
  return subset;
}
}  // namespace


Subset AppendGeoSphereSuset(VertexPack* vp, IndexPack* ipack,
                            const GeoSphereParam& p, const Matrix4& mat) {
  Subset subset = AppendGeoSphereVertexSuset(vp, ipack, p, mat);
  subset.index_base = ipack->index();
  AppendUpGeoTaperIndexData(0, ipack, p.slice);
  for (int i = 1; i < p.stack - 2; ++i) {
    int line1 = 1 + (p.slice + 1) * (i - 1);
    int line2 = 1 + (p.slice + 1) * i;
    GenTriStripIndex(line1, line2, p.slice + 1, ipack);
  }

  int32_t last = subset.vertex_count - 1;
  AppendBottomGeoTaperIndexData(last, ipack, p.slice);
  subset.index_count = ipack->index() - subset.index_base;
  CalcIndexedTriangleNormal(vp->data(), ipack->data(), subset);
  return subset;
}

namespace {
void UpdateSphereNormal(VertexPack* vpack, int32_t stack, int32_t slice, Subset subset) {
  const int32_t kIndexCount = CalcSphereIndexCount(stack, slice);
  IndicesDataPtr ptr(new IndicesData(kIndexCount));
  IndexPack ipack(ptr);
  subset.index_base = ipack.index();
  AppendUpGeoTaperIndexData(0, &ipack, slice);
  for (int i = 1; i < stack - 2; ++i) {
    int line1 = 1 + (slice + 1) * (i - 1);
    int line2 = 1 + (slice + 1) * i;
    GenTriStripIndex(line1, line2, slice + 1, &ipack);
  }

  int32_t last = subset.vertex_count - 1;
  AppendBottomGeoTaperIndexData(last, &ipack, slice);
  subset.index_count = ipack.index() - subset.index_base;
  CalcIndexedTriangleNormal(vpack->data(), ipack.data(), subset);
}
}

Subset AppendGeoSphereFrameSuset(VertexPack* vp, IndexPack* ipack,
                                 const GeoSphereParam& p, const Matrix4& mat) {
  const int32_t kVertexCount = CalcSphereVertexCount(p.stack, p.slice);
  Subset subset = AppendGeoSphereVertexSuset(vp, ipack, p, mat);
  UpdateSphereNormal(vp, p.stack, p.slice, subset);

  subset.index_base = ipack->index();
  for (int i = 0; i < p.slice + 1; ++i) {
    CHECK(ipack->WriteAndAdvance(0));
    CHECK(ipack->WriteAndAdvance(1 + i));
  }

  for (int i = 1; i < p.stack - 2; ++i) {
    for (int j = 0; j < p.slice; ++j) {
      CHECK(ipack->WriteAndAdvance(1 + (i - 1) * p.slice + j));
      CHECK(ipack->WriteAndAdvance(1 + i * p.slice + j));
    }
  }

  for (int i = 0; i < p.slice + 1; ++i) {
    CHECK(ipack->WriteAndAdvance(kVertexCount - 1));
    CHECK(ipack->WriteAndAdvance(kVertexCount - 1 - (i + 1)));
  }

  // horizontal line
  for (int i = 1; i < p.stack - 1; ++i) {
    for (int j = 0; j < p.slice + 1; ++j) {
      CHECK(ipack->WriteAndAdvance(1 + (i - 1) * (p.slice + 1) + j));
      CHECK(ipack->WriteAndAdvance(1 + (i - 1) * (p.slice + 1) + (j + 1)));
    }
  }

  subset.index_count = ipack->index() - subset.index_base;
  subset.primitive = kLineList;
  return subset;
}

void AppendGeoSphereData(EntityData* data, const GeoSphereParam& p, 
                         const Matrix4& mat) {
  const int32_t kVertexCount = CalcSphereVertexCount(p.stack, p.slice);
  const int32_t kIndexCount = CalcSphereIndexCount(p.stack, p.slice);
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  vpack.move(data->vdata()->vertex_count() - kVertexCount);
  ipack.move(data->idata()->count() - kIndexCount);
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

void AppendGeoSphereFrameData(EntityData* data, const GeoSphereParam& p, 
                              const Matrix4& mat) {
  const int32_t kVertexCount = CalcSphereVertexCount(p.stack, p.slice);
  const int32_t kIndexCount = CalcSphereFrameIndexCount(p.stack, p.slice);
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  vpack.move(data->vdata()->vertex_count() - kVertexCount);
  ipack.move(data->idata()->count() - kIndexCount);
  Subset subset = AppendGeoSphereFrameSuset(&vpack, &ipack, p, mat);
  data->AddSubset(subset);
}

EntityDataPtr CreateSphereFrame(VertexDesc* desc, const GeoSphereParam& p,
                                const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata, idata));
  AppendGeoSphereFrameData(data.get(), p, mat);
  return data;
}
}  // namespace azer
