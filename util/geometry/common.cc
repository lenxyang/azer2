#include "azer/util/geometry/common.h"

#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"

namespace azer {
int32_t CalcGeoRoundIndexCount(int slice) { return slice * 3;}
int32_t CalcGeoRoundVertexCount(int slice) { return 1 + slice + 1;}

int32_t AppendUpGeoTaperIndexData(int32_t base, IndexPack* ipack, int slice) {
  int32_t begin = ipack->index();
  for (int i = 0; i < slice; ++i) {
    int index1 = 1 + i;
    int index2 = 1 + i + 1;
    CHECK(ipack->WriteAndAdvance(base));
    CHECK(ipack->WriteAndAdvance(base + index2));
    CHECK(ipack->WriteAndAdvance(base + index1));
  }
  return ipack->index() - begin;
}

int32_t AppendBottomGeoTaperIndexData(int32_t last, IndexPack* ipack, int slice) {
  int32_t begin = ipack->index();
  for (int i = 0; i < slice; ++i) {
    int index1 = last - i - 2;
    int index2 = last - i - 1;
    CHECK(ipack->WriteAndAdvance(last));
    CHECK(ipack->WriteAndAdvance(index1));
    CHECK(ipack->WriteAndAdvance(index2));
  }
  return ipack->index() - begin;
}

Subset AppendGeoRoundData(VertexPack* vpack, IndexPack* ipack, float radius, 
                          int slice, const Matrix4& mat) {
  const int32_t kVertexNum = CalcGeoRoundVertexCount(slice);
  Subset subset;
  VertexPos vpos(0, 0), npos;
  GetSemanticIndex("normal", 0, vpack->desc(), &npos);
  float degree = 360.0f / (float)slice;
  subset.vertex_base = vpack->index();
  Vector4 pos = mat * Vector4(0, 0, 0, 1.0f);
  vpack->WriteVector3Or4(pos, vpos);
  UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
  vpack->WriteVector3Or4(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f), npos);
  vpack->next(1);
  for (int i = 1; i < kVertexNum; ++i) {
    float x = cos(Degree(i * degree)) * radius;
    float z = sin(Degree(i * degree)) * radius;
    Vector4 pos = std::move(mat * Vector4(x, 0, z, 1.0f));
    Vector4 normal = std::move(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f));
    vpack->WriteVector3Or4(pos, vpos);
    UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
    vpack->WriteVector3Or4(normal, npos);
    vpack->next(1);
  }
  subset.vertex_count = vpack->index() - subset.vertex_base;
  subset.index_base = ipack->index();
  subset.index_count = AppendUpGeoTaperIndexData(0, ipack, slice);
  subset.primitive = kTriangleList;
  return subset;
}

void AppendGeoRoundSubset(EntityData* data, float radius, int slice, 
                        const Matrix4& mat) {
  const int32_t kVertexCount = CalcGeoRoundVertexCount(slice);
  const int32_t kIndexCount = CalcGeoRoundIndexCount(slice);
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  vpack.data()->extend(kVertexCount);
  ipack.data()->extend(kIndexCount);
  vpack.move(vpack.data()->vertex_count() - kVertexCount);
  ipack.move(ipack.count() - kIndexCount);
  Subset subset = AppendGeoRoundData(&vpack, &ipack, radius, slice, mat); 
  data->AddSubset(subset);
}

Subset AppendGeoCircleData(VertexPack* vpack, float radius, 
                           int slice, const Matrix4& mat) {
  Subset subset;
  VertexPos vpos(0, 0);
  float degree = 360.0f / (float)slice;
  subset.vertex_base = vpack->index();
  Vector4 pos = mat * Vector4(0, 0, 0, 1.0f);
  for (int i = 1; i < slice + 1; ++i) {
    float x = cos(Degree(i * degree)) * radius;
    float z = sin(Degree(i * degree)) * radius;
    Vector4 pos = std::move(mat * Vector4(x, 0, z, 1.0f));
    Vector4 normal = std::move(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f));
    vpack->WriteVector3Or4(pos, vpos);
    UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
    vpack->next(1);
  }
  subset.vertex_count = vpack->index() - subset.vertex_base;
  subset.index_count = 0;
  subset.primitive = kLineStrip;
  return subset;
}

void AppendGeoCircleSubset(EntityData* data, float radius, int slice, 
                           const Matrix4& mat) {
  const int32_t kVertexCount = slice + 1;
  VertexPack vpack(data->vdata());
  vpack.data()->extend(kVertexCount);
  vpack.move(vpack.data()->vertex_count() - kVertexCount);
  Subset subset = AppendGeoCircleData(&vpack, radius, slice, mat); 
  data->AddSubset(subset);
}

void GenTriStripIndex(int32_t line1, int32_t line2, int32_t vertex_num, IndexPack* ipack) {
  for (int i = 0; i < vertex_num - 1; ++i) {
    int index1 = i;
    int index2 = i + 1;
    CHECK(ipack->WriteAndAdvance(line1 + index1));
    CHECK(ipack->WriteAndAdvance(line1 + index2));
    CHECK(ipack->WriteAndAdvance(line2 + index1));

    CHECK(ipack->WriteAndAdvance(line1 + index2));
    CHECK(ipack->WriteAndAdvance(line2 + index2));
    CHECK(ipack->WriteAndAdvance(line2 + index1));
  }
}

void UpdateVertexBounds(const Vector3 pos, Vector3* vmin, Vector3* vmax) {
  if (pos.x < vmin->x) vmin->x = pos.x;
  if (pos.y < vmin->y) vmin->y = pos.y;
  if (pos.z < vmin->z) vmin->z = pos.z;
  if (pos.x > vmax->x) vmax->x = pos.x;
  if (pos.y > vmax->y) vmax->y = pos.y;
  if (pos.z > vmax->z) vmax->z = pos.z;
}
void UpdateVertexBounds(const Vector4 pos, Vector3* vmin, Vector3* vmax) {
  if (pos.x < vmin->x) vmin->x = pos.x;
  if (pos.y < vmin->y) vmin->y = pos.y;
  if (pos.z < vmin->z) vmin->z = pos.z;
  if (pos.x > vmax->x) vmax->x = pos.x;
  if (pos.y > vmax->y) vmax->y = pos.y;
  if (pos.z > vmax->z) vmax->z = pos.z;
}
}  // namespace azer
