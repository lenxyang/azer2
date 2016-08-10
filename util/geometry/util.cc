#include "azer/util/geometry/util.h"

#include "azer/render/bounding_volumn.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/index_pack.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/vertex_pack.h"

namespace azer {
void CalcIndexedTriangleNormal(VertexData* vbd, IndicesData* idata) {
  VertexPos npos;
  if (!GetSemanticIndex("normal", 0, vbd->vertex_desc(), &npos)) {
    return;
  }

  std::vector<float> used;
  std::vector<Vector4> normals;
  used.resize(vbd->vertex_count());
  normals.resize(vbd->vertex_count());
  
  VertexPack vpack(vbd);
  vpack.first();
  for (int i = 0; i < vbd->vertex_count(); ++i) {
    DCHECK(!vpack.end());
    vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 0.0f), npos);
    vpack.next(1);
    used[i] = 0.0f;
    normals[i] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  vpack.first();
  IndexPack ipack(idata);
  for (int i = 0; i < idata->count(); i+=3) {
    uint32_t idx1 = ipack.ReadAndAdvanceOrDie();
    uint32_t idx2 = ipack.ReadAndAdvanceOrDie();
    uint32_t idx3 = ipack.ReadAndAdvanceOrDie();
    Vector3 p1, p2, p3;
    CHECK(vpack.move(idx1));
    vpack.ReadVector3Or4(&p1, VertexPos(0, 0));
    CHECK(vpack.move(idx2));
    vpack.ReadVector3Or4(&p2, VertexPos(0, 0));
    CHECK(vpack.move(idx3));
    vpack.ReadVector3Or4(&p3, VertexPos(0, 0));
    used[idx1] += 1.0f;
    used[idx2] += 1.0f;
    used[idx3] += 1.0f;

    Vector4 normal = Vector4(CalcPlaneNormal(p1, p2, p3), 0.0f);
    normals[idx1] += normal;
    normals[idx2] += normal;
    normals[idx3] += normal;
  }

  vpack.first();
  for (int i = 0; i < static_cast<int>(normals.size()); ++i) {
    Vector4 normal = normals[i] / used[i];
    normal.Normalize();
    vpack.WriteVector3Or4(normal, npos);
    vpack.next(1);
  }
}

void CalcTriangleListNormal(SlotVertexData* vbd, int* indices) {
  VertexPos npos;
  if (!GetSemanticIndex("normal", 0, vbd->vertex_desc(), &npos)) {
    return;
  }

  std::vector<float> used;
  std::vector<Vector4> normals;
  used.resize(vbd->vertex_count());
  normals.resize(vbd->vertex_count());
  
  VertexPack vpack(vbd);
  vpack.first();
  for (int i = 0; i < vbd->vertex_count(); ++i) {
    vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 0.0f), npos);
    vpack.next(1);
    used[i] = 0.0f;
    normals[i] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  vpack.first();
  for (int i = 0; i < vbd->vertex_count(); i+=3) {
    int idx1 = *(indices + i);
    int idx2 = *(indices + i + 1);
    int idx3 = *(indices + i + 2);
    Vector3 p1, p2, p3;
    CHECK(vpack.move(idx1));
    vpack.ReadVector3Or4(&p1, VertexPos(0, 0));
    CHECK(vpack.move(idx2));
    vpack.ReadVector3Or4(&p2, VertexPos(0, 0));
    CHECK(vpack.move(idx3));
    vpack.ReadVector3Or4(&p3, VertexPos(0, 0));
    used[idx1] += 1.0f;
    used[idx2] += 1.0f;
    used[idx3] += 1.0f;

    Vector4 normal = Vector4(CalcPlaneNormal(p1, p2, p3), 0.0f);
    normals[idx1] += normal;
    normals[idx2] += normal;
    normals[idx3] += normal;
  }

  for (size_t i = 0; i < normals.size(); ++i) {
    normals[i] /= used[i];
  }

  vpack.first();
  for (size_t i = 0; i < normals.size(); ++i) {
    Vector4 normal = normals[i] / used[i];
    normal.Normalize();
    vpack.WriteVector3Or4(normal, npos);
    vpack.next(1);
  }
}

void CalcIndexedTriangleListTangentAndBinormal(VertexData* vd, IndicesData* id) {
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
  
  for (int32_t i = 0; i < ipack.count(); i+=3) {
    uint32_t index[3];
    CHECK(ipack.ReadAndAdvance(index));
    CHECK(ipack.ReadAndAdvance(index + 1));
    CHECK(ipack.ReadAndAdvance(index + 2));
    Vector3 p1, p2, p3;
    Vector2 t1, t2, t3;
    Vector3 tangent, binormal, normal;
    pickle.move(index[0]);
    pickle.ReadVector3Or4(&p1, ppos);
    pickle.ReadVector2(&t1, tpos);
    pickle.move(index[1]);
    pickle.ReadVector3Or4(&p2, ppos);
    pickle.ReadVector2(&t2, tpos);
    pickle.move(index[2]);
    pickle.ReadVector3Or4(&p3, ppos);
    pickle.ReadVector2(&t3, tpos);

    CalcTriangleTBN(p1, t1, p2, t2, p3, t3, &tangent, &normal, &binormal);
    for (int j = 0; j < 3; ++j) {
      pickle.move(index[j]);
      pickle.WriteVector3Or4(Vector4(tangent, 0.0f), tagentpos);
      pickle.WriteVector3Or4(Vector4(normal, 0.0f), npos);
    }
  }
}


EntityPtr CreateGeoPointsList(PrimitiveTopology primitive, const Vector3* points,
                              int32_t count, VertexDesc* desc, const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, count));
  VertexPack vpack(vdata.get());
  vpack.first();
  Vector3 vmin(mat * Vector4(points[0], 1.0));
  Vector3 vmax(mat * Vector4(points[0], 1.0));
  for (int32_t i = 0; i < count; ++i) {
    Vector4 vec(mat * Vector4(points[i], 1.0f));
    vpack.WriteVector3Or4(vec, VertexPos(0, 0));
    UpdateVMinAndVMax(Vector3(vec), &vmin, &vmax);
    vpack.next(1);
  }

  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata.get());
  EntityPtr entity(new Entity(vb.get()));
  entity->set_vmin(vmin);
  entity->set_vmax(vmax);
  Subset subset(0, vb->vertex_count(), 0, 0);
  subset.primitive = primitive;
  entity->AddSubset(subset);
  return entity;
}

}
