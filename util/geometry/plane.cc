#include "azer/util/geometry/plane.h"

#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"
#include "azer/util/geometry/common.h"

namespace azer {
namespace {
int32_t CalcVertexCount(int32_t row, int column) {
  return (row + 1) * (column + 1);
};
int32_t CalcIndexCount(int32_t row, int column) {
  return row * column * 6;
};
}
Subset AppendGeoPlaneData(VertexPack* vp, IndexPack* ipack, 
                          const PlaneParam& p, const Matrix4& mat) {
  VertexPos vpos(0, 0), normpos, texpos;
  GetSemanticIndex("texcoord", 0, vp->desc(), &texpos);
  GetSemanticIndex("normal", 0, vp->desc(), &normpos);
  Subset subset;
  subset.vertex_base = vp->index();

  float cell_length = p.width / p.column;
  float cell_depth = p.depth / p.row;
  for (int32_t i = 0; i < p.column + 1; ++i) {
    float tu = i * 1.0f / p.column;
    float x = i * cell_length;
    for (int32_t j = 0; j < p.row + 1; ++j) {
      float z = j * cell_depth;
      float tv = j * 1.0f / p.row;
      Vector4 pos = mat * Vector4(x, 0.0f, z, 1.0f);
      vp->WriteVector3Or4(pos, vpos);
      vp->WriteVector3Or4(Vector4(0.0f, 1.0, 0.0f, 0.0), normpos);
      vp->WriteVector2(Vector2(tu, tv), texpos);
      UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
      vp->next(1);
    }
  }
  subset.vertex_count = vp->index() - subset.vertex_base;

  subset.index_base = ipack->index();
  for (int32_t i = 0; i < p.row; ++i) {
    int32_t line1 = subset.vertex_base + i * (p.row + 1);
    int32_t line2 = subset.vertex_base + (i + 1) * (p.row + 1);
    GenTriStripIndex(line1, line2, (p.column + 1), ipack);
  }
  subset.primitive = kTriangleList;
  subset.index_count = ipack->index() - subset.index_base;
  return subset;
}

void AppendGeoPlaneSubset(EntityData* data, const PlaneParam& p, const Matrix4& mat) {
  const int32_t kIndexCount = CalcIndexCount(p.row, p.column);
  const int32_t kVertexCount = CalcVertexCount(p.row, p.column);
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  vpack.move(data->vdata()->vertex_count() - kVertexCount);
  ipack.move(data->idata()->count() - kIndexCount);
  Subset subset = AppendGeoPlaneData(&vpack, &ipack, p, mat);
  data->AddSubset(subset);
}

EntityDataPtr CreatePlane(VertexDesc* desc, const PlaneParam& param,
                          const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoPlaneSubset(data.get(), param, mat);
  return data;
}

Subset AppendGeoSquareData(VertexPack* vp, IndexPack* ipack, 
                           float length, int32_t slice,
                           const Matrix4& mat) {
  VertexPos vpos(0, 0), normpos, texpos;
  GetSemanticIndex("texcoord", 0, vp->desc(), &texpos);
  GetSemanticIndex("normal", 0, vp->desc(), &normpos);
  Subset subset;
  subset.vertex_base = vp->index();

  float cell_length = length / slice;
  for (int32_t i = 0; i < slice + 1; ++i) {
    float tu = i * 1.0f / slice;
    float x = i * cell_length;
    for (int32_t j = 0; j < slice + 1; ++j) {
      float z = j * cell_length;
      float tv = j * 1.0f / slice;
      Vector4 pos = mat * Vector4(x, 0.0f, z, 1.0f);
      vp->WriteVector3Or4(pos, vpos);
      vp->WriteVector3Or4(Vector4(0.0f, 1.0, 0.0f, 0.0), normpos);
      vp->WriteVector2(Vector2(tu, tv), texpos);
      UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
      vp->next(1);
    }
  }
  subset.vertex_count = vp->index() - subset.vertex_base;

  subset.index_base = ipack->index();
  for (int32_t i = 0; i < slice; ++i) {
    int32_t line1 = subset.vertex_base + i * (slice + 1);
    int32_t line2 = subset.vertex_base + (i + 1) * (slice + 1);
    GenTriStripIndex(line1, line2, slice, ipack);
  }
  subset.primitive = kTriangleList;
  subset.index_count = ipack->index() - subset.index_base;
  return subset;
}

void AppendGeoSquareSubset(EntityData* data, float length, int32_t slice,
                         const Matrix4& mat) {
  const int32_t kIndexCount = CalcIndexCount(slice + 1, slice + 1);
  const int32_t kVertexCount = CalcVertexCount(slice + 1, slice + 1);
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  vpack.move(data->vdata()->vertex_count() - kVertexCount);
  ipack.move(data->idata()->count() - kIndexCount);
  Subset subset = AppendGeoSquareData(&vpack, &ipack, length, slice, mat);
  data->AddSubset(subset);
}

EntityDataPtr CreateSquare(VertexDesc* desc, float length, int32_t slice,
                           const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoSquareSubset(data.get(), length, slice, mat);
  return data;
}
}  // namespace azer
