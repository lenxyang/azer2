#include "azer/util/geometry/common.h"

#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"

namespace azer {
int32 CalcGeoRoundIndexCount(int slice) { return slice * 3;}
int32 CalcGeoRoundVertexCount(int slice) { return 1 + slice + 1;}

void AppendGeoRoundIndexData(IndexPack* ipack, int slice, Subset* subset) {
  subset->index_base = ipack->index();
  for (int i = 0; i < slice; ++i) {
    int index1 = 1 + (i + 1) % slice;
    int index2 = 1 + i;
    CHECK(ipack->WriteAndAdvance(0));
    CHECK(ipack->WriteAndAdvance(index1));
    CHECK(ipack->WriteAndAdvance(index2));
  }
  subset->index_count = ipack->index() - subset->index_base;
}

Subset AppendGeoRoundData(VertexPack* vpack, IndexPack* ipack, float radius, 
                          int slice, const Matrix4& mat) {
  const int32 kVertexNum = CalcGeoRoundVertexCount(slice);
  vpack->data()->extend(kVertexNum);
  Subset subset;
  VertexPos vpos(0, 0), npos;
  GetSemanticIndex("normal", 0, vpack->desc(), &npos);
  float degree = 360.0f / (float)slice;
  subset.vertex_base = vpack->index();
  vpack->WriteVector3Or4(mat * Vector4(0, 0, 0, 1.0f), vpos);
  vpack->WriteVector3Or4(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f), npos);
  vpack->next(1);
  for (int i = 1; i < kVertexNum; ++i) {
    float x = cos(Degree(i * degree)) * radius;
    float z = sin(Degree(i * degree)) * radius;
    vpack->WriteVector3Or4(mat * Vector4(x, 0, z, 1.0f), vpos);
    vpack->WriteVector3Or4(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f), npos);
    vpack->next(1);
  }
  subset.vertex_count = vpack->index() - subset.vertex_base;
  AppendGeoRoundIndexData(ipack, slice, &subset);
  subset.primitive = kTriangleList;
  return subset;
}
}  // namespace azer
