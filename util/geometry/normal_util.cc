#include "azer/util/geometry/normal_util.h"

#include "azer/render/index_pack.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
void CalcIndexedTriangleNormal(VertexData* vbd, IndicesData* idata,
                               const Subset& subset) {
  VertexPos npos;
  if (!GetSemanticIndex("normal", 0, vbd->vertex_desc(), &npos)) {
    return;
  }

  std::vector<float> used;
  std::vector<Vector4> normals;
  used.resize(subset.vertex_count);
  normals.resize(subset.vertex_count);
  
  VertexPack vpack(vbd);
  vpack.move(subset.vertex_base);
  for (int i = 0; i < subset.vertex_count; ++i) {
    DCHECK(!vpack.end());
    vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 0.0f), npos);
    vpack.next(1);
    used[i] = 0.0f;
    normals[i] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  IndexPack ipack(idata);
  ipack.move(subset.index_base);
  for (int i = 0; i < subset.index_count; i+=3) {
    uint32 idx1 = ipack.ReadAndAdvanceOrDie();
    uint32 idx2 = ipack.ReadAndAdvanceOrDie();
    uint32 idx3 = ipack.ReadAndAdvanceOrDie();
    DCHECK_LT(idx1, subset.vertex_count);
    DCHECK_LT(idx2, subset.vertex_count);
    DCHECK_LT(idx3, subset.vertex_count);
    Vector3 p1, p2, p3;
    CHECK(vpack.move(idx1 + subset.vertex_base));
    vpack.ReadVector3Or4(&p1, VertexPos(0, 0));
    CHECK(vpack.move(idx2 + subset.vertex_base));
    vpack.ReadVector3Or4(&p2, VertexPos(0, 0));
    CHECK(vpack.move(idx3 + subset.vertex_base));
    vpack.ReadVector3Or4(&p3, VertexPos(0, 0));
    used[idx1] += 1.0f;
    used[idx2] += 1.0f;
    used[idx3] += 1.0f;

    Vector4 normal = Vector4(CalcPlaneNormal(p1, p2, p3), 0.0f);
    normals[idx1] += normal;
    normals[idx2] += normal;
    normals[idx3] += normal;
  }

  vpack.move(subset.vertex_base);
  for (int i = 0; i < normals.size(); ++i) {
    Vector4 normal = normals[i] / used[i];
    normal.Normalize();
    vpack.WriteVector3Or4(normal, npos);
    vpack.next(1);
  }
}

void CalcIndexedTriangleListTangentAndBinormal(VertexData* vd, IndicesData* id, 
                                               const Subset& subset) {
  VertexPack pickle(vd);
  IndexPack ipack(id);

  const VertexDesc* desc = vd->vertex_desc();
  VertexPos ppos, npos, tpos, binpos, tagentpos;
  bool has_pos = GetSemanticIndex("position", 0, desc, &ppos);
  bool has_normal = GetSemanticIndex("normal", 0, desc, &npos);
  bool has_tex = GetSemanticIndex("texcoord", 0, desc, &tpos);
  bool has_tagent = GetSemanticIndex("tangent", 0, desc, &tagentpos);
  if (!has_pos ||  !has_normal || !has_tex || !has_tagent) {
    return;
  }
  
  ipack.move(subset.index_base);
  for (int32 i = 0; i < subset.index_count; i+=3) {
    uint32 index[3];
    CHECK(ipack.ReadAndAdvance(index));
    CHECK(ipack.ReadAndAdvance(index + 1));
    CHECK(ipack.ReadAndAdvance(index + 2));
    Vector3 p1, p2, p3;
    Vector2 t1, t2, t3;
    Vector3 tangent, binormal, normal;
    pickle.move(subset.vertex_base + index[0]);
    pickle.ReadVector3Or4(&p1, ppos);
    pickle.ReadVector2(&t1, tpos);
    pickle.move(subset.vertex_base + index[1]);
    pickle.ReadVector3Or4(&p2, ppos);
    pickle.ReadVector2(&t2, tpos);
    pickle.move(subset.vertex_base + index[2]);
    pickle.ReadVector3Or4(&p3, ppos);
    pickle.ReadVector2(&t3, tpos);

    CalcTriangleTBN(p1, t1, p2, t2, p3, t3, &tangent, &normal, &binormal);
    for (int j = 0; j < 3; ++j) {
      pickle.move(subset.vertex_base + index[j]);
      pickle.WriteVector3Or4(Vector4(tangent, 0.0f), tagentpos);
      pickle.WriteVector3Or4(Vector4(normal, 0.0f), npos);
    }
  }
}
}  // namespace azer
