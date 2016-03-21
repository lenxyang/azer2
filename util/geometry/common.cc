#include "azer/util/geometry/common.h"

#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"

namespace azer {
int32 CalcGeoRoundIndexCount(int slice) { return slice * 3;}
int32 CalcGeoRoundVertexCount(int slice) { return 1 + slice + 1;}

int32 AppendUpGeoTaperIndexData(int32 base, IndexPack* ipack, int slice) {
  int32 begin = ipack->index();
  for (int i = 0; i < slice; ++i) {
    int index1 = 1 + i + 1;
    int index2 = 1 + i;
    CHECK(ipack->WriteAndAdvance(base));
    CHECK(ipack->WriteAndAdvance(base + index1));
    CHECK(ipack->WriteAndAdvance(base + index2));
  }
  return ipack->index() - begin;
}

int32 AppendBottomGeoTaperIndexData(int32 base, IndexPack* ipack, int slice) {
  int32 begin = ipack->index();
  for (int i = 0; i < slice; ++i) {
    int index1 = i;
    int index2 = i + 1;
    CHECK(ipack->WriteAndAdvance(base + slice));
    CHECK(ipack->WriteAndAdvance(base + index2));
    CHECK(ipack->WriteAndAdvance(base + index1));
  }
  return ipack->index() - begin;
}

Subset AppendGeoRoundData(VertexPack* vpack, IndexPack* ipack, float radius, 
                          int slice, const Matrix4& mat) {
  const int32 kVertexNum = CalcGeoRoundVertexCount(slice);
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

void AppendGeoRoundData(EntityData* data, float radius, int slice, 
                        const Matrix4& mat) {
  const int32 kVertexCount = CalcGeoRoundVertexCount(slice);
  const int32 kIndexCount = CalcGeoRoundIndexCount(slice);
  VertexPack vpack(data->vdata());
  IndicesPack ipack(data->idata());
  vpack->extend(kVertexCount);
  ipack->extend(kIndexCount);
  vpack->move(vpack->count() - kVertexCount);
  ipack->move(ipack->count() - kIndexCount);
  Subset subset = AppendGeoRoundData(&vpack, &ipack, radius, slice, mat); 
  data->AddSubset(subset);
}

void GenTriStripIndex(int32 line1, int32 line2, int32 vertex_num, IndexPack* ipack) {
  for (int i = 0; i < vertex_num - 1; ++i) {
    int index1 = i;
    int index2 = i + 1;
    CHECK(ipack->WriteAndAdvance(line1 + index2));
    CHECK(ipack->WriteAndAdvance(line1 + index1));
    CHECK(ipack->WriteAndAdvance(line2 + index1));

    CHECK(ipack->WriteAndAdvance(line1 + index2));
    CHECK(ipack->WriteAndAdvance(line2 + index1));
    CHECK(ipack->WriteAndAdvance(line2 + index2));
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
