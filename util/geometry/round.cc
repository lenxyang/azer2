#include "azer/util/geometry/round.h"

namespace azer {
void WriteRoundData(Subset* subset, VertexPack* vpack, IndexPack* ipack, 
                    float radius, int32 slice, const Matrix4& mat) {
  subset->vertex_base = vpack->index();
  subset->vertex_count = kVertexNum;
  subset->index_base = ipack->index();
  subset->index_count = slice * 3;

  const int32 kVertexNum = 1 + slice + 1;

  VertexPos npos;
  GetSemanticIndex("normal", 0, vpack->desc(), &npos);
  vpack->WriteVector3Or4(mat * Vector4(0, 0, 0, 1.0f), VertexPos(0, 0));
  vpack->WriteVector3Or4(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f), npos);
  vpack->next(1);
  for (int i = 1; i < kVertexNum; ++i) {
    float x = cos(Degree(i * degree)) * radius;
    float z = sin(Degree(i * degree)) * radius;
    vpack->WriteVector3Or4(mat * Vector4(x, 0, z, 1.0f), VertexPos(0, 0));
    vpack->WriteVector3Or4(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f), npos);
    vpack->next(1);
  }

  for (int i = 0; i < slice; ++i) {
    int index1 = 1 + (i + 1) % slice;
    int index2 = 1 + i;
    CHECK(ipack.WriteAndAdvance(0));
    CHECK(ipack.WriteAndAdvance(index1));
    CHECK(ipack.WriteAndAdvance(index2));
  }
}
}  // namespace azer
