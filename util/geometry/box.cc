#include "azer/util/geometry/cylinder.h"

#include "azer/render/index_pack.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/vertex_buffer.h"
#include "azer/util/geometry/common.h"
#include "azer/util/geometry/cone.h"
#include "azer/util/geometry/normal_util.h"

namespace azer {
namespace {
const Vector4 box_position[] = {
  Vector4(-0.5f,  0.5f,  0.5f, 1.0f),
  Vector4( 0.5f,  0.5f,  0.5f, 1.0f),
  Vector4( 0.5f, -0.5f,  0.5f, 1.0f),
  Vector4(-0.5f, -0.5f,  0.5f, 1.0f),
  Vector4(-0.5f,  0.5f, -0.5f, 1.0f),
  Vector4( 0.5f,  0.5f, -0.5f, 1.0f),
  Vector4( 0.5f, -0.5f, -0.5f, 1.0f),
  Vector4(-0.5f, -0.5f, -0.5f, 1.0f),
};

const Vector2 texcoord0[] = {
  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 1.0f),
  Vector2(1.0f, 0.0f),
  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 0.0f),
  Vector2(1.0f, 1.0f),

  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 1.0f),
  Vector2(1.0f, 0.0f),
  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 0.0f),
  Vector2(1.0f, 1.0f),

  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 1.0f),
  Vector2(1.0f, 0.0f),
  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 0.0f),
  Vector2(1.0f, 1.0f),

  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 1.0f),
  Vector2(1.0f, 0.0f),
  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 0.0f),
  Vector2(1.0f, 1.0f),

  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 1.0f),
  Vector2(1.0f, 0.0f),
  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 0.0f),
  Vector2(1.0f, 1.0f),

  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 1.0f),
  Vector2(0.0f, 1.0f),
  Vector2(0.0f, 0.0f),
  Vector2(1.0f, 0.0f),
  Vector2(1.0f, 1.0f),
};

Vector4 box_normal[] = {
  Vector4(0.0f, 0.0f, 1.0f, 0.0f),
  Vector4(1.0f, 0.0f, 0.0f, 0.0f),
  Vector4(0.0f, 0.0f, -1.0f, 0.0f),
  Vector4(-1.0f, 0.0f, 0.0f, 0.0f),
  Vector4(0.0f,  1.0f, 0.0f, 0.0f),
  Vector4(0.0f,  -1.0f, 0.0f, 0.0f),
};

int indices[] = {0, 2, 1, 0, 3, 2,  // front
                 1, 6, 5, 1, 2, 6,  // right
                 5, 7, 4, 5, 6, 7,  // back
                 4, 3, 0, 4, 7, 3,  // left
                 4, 1, 5, 4, 0, 1,  // top
                 3, 6, 2, 3, 7, 6}; // bottom
int32_t edge_indices[] = {0, 2, 2, 1, 1, 4, 4, 0,
                          0, 14, 2, 8, 1, 7, 4, 13,
                          14, 8, 8, 7, 7, 13, 13, 14};
int32_t CalcBoxVertexCount() { return static_cast<int32_t>(arraysize(indices));}
int32_t CalcBoxIndexCount() { return 0;}
int32_t CalcBoxFrameIndexCount() { return static_cast<int32_t>(
    arraysize(edge_indices));}

void CalcTriangleListNormal(int32_t base, int32_t vcount, int* indices,
                            VertexPack* vpack) {
  VertexPos npos;
  if (!GetSemanticIndex("normal", 0, vpack->desc(), &npos)) {
    return;
  }

  std::vector<float> used;
  std::vector<Vector4> normals;
  used.resize(vcount);
  normals.resize(vcount);
  
  vpack->move(base);
  for (int i = 0; i < vcount; ++i) {
    vpack->WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 0.0f), npos);
    vpack->next(1);
    used[i] = 0.0f;
    normals[i] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  vpack->move(base);
  for (int i = 0; i < vcount; i+=3) {
    int idx1 = *(indices + i);
    int idx2 = *(indices + i + 1);
    int idx3 = *(indices + i + 2);
    Vector3 p1, p2, p3;
    CHECK(vpack->move(idx1));
    vpack->ReadVector3Or4(&p1, VertexPos(0, 0));
    CHECK(vpack->move(idx2));
    vpack->ReadVector3Or4(&p2, VertexPos(0, 0));
    CHECK(vpack->move(idx3));
    vpack->ReadVector3Or4(&p3, VertexPos(0, 0));
    used[idx1] += 1.0f;
    used[idx2] += 1.0f;
    used[idx3] += 1.0f;

    Vector4 normal = Vector4(CalcPlaneNormal(p1, p2, p3), 0.0f);
    normals[idx1] += normal;
    normals[idx2] += normal;
    normals[idx3] += normal;
  }

  for (int i = 0; i < normals.size(); ++i) {
    normals[i] /= used[i];
  }

  vpack->move(base);
  for (int i = 0; i < normals.size(); ++i) {
    Vector4 normal = normals[i] / used[i];
    normal.Normalize();
    vpack->WriteVector3Or4(normal, npos);
    vpack->next(1);
  }
}
}  // namespace


int32_t CreateBoxFrameIndicesData(IndexPack* ipack) {
  for (uint32_t i = 0; i < arraysize(edge_indices); ++i) {
    CHECK(ipack->WriteAndAdvance(edge_indices[i]));
  }
  return static_cast<int32_t>(arraysize(edge_indices));
}

Subset AppendGeoHexaHedronSubset(VertexPack* vpack, const Vector4* posvec,
                                 const Matrix4& mat) {
  VertexPos normal_pos, tex0_pos;
  bool HasNormal = GetSemanticIndex("normal", 0, vpack->desc(), &normal_pos);
  GetSemanticIndex("texcoord", 0, vpack->desc(), &tex0_pos);
  Subset subset;
  subset.vertex_base = vpack->index();
  for (int i = 0; i < static_cast<int>(arraysize(indices)); ++i) {
    int index = indices[i];
    DCHECK(!vpack->end());
    Vector4 pos = mat * posvec[index];
    vpack->WriteVector3Or4(pos, VertexPos(0, 0));
    UpdateVertexBounds(pos, &subset.vmin, &subset.vmax);
    vpack->WriteVector2(texcoord0[index], tex0_pos);
    vpack->next(1);
  }
  subset.vertex_count = vpack->index() - subset.vertex_base;

  if (HasNormal) {
    CalcTriangleListNormal(subset.vertex_base, subset.vertex_count, 
                           indices, vpack);
  }

  return subset;
}

Subset AppendGeoBoxSubset(VertexPack* vp, IndexPack* ipack, 
                          const Matrix4& mat) {
  return AppendGeoHexaHedronSubset(vp, box_position, mat);
}

Subset AppendGeoBoxFrameSubset(VertexPack* vp, IndexPack* ipack, 
                               const Matrix4& mat) {
  Subset subset = AppendGeoHexaHedronSubset(vp, box_position, mat);
  subset.index_base = ipack->index();
  subset.index_count = CreateBoxFrameIndicesData(ipack);
  subset.primitive = kLineList;
  return subset;
}

void AppendGeoBoxData(EntityData* data, const Matrix4& mat) {
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  const int32_t kVertexCount = CalcBoxVertexCount();
  const int32_t kIndexCount = 0;
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  vpack.move(data->vdata()->vertex_count() - kVertexCount);
  Subset sub = AppendGeoBoxSubset(&vpack, &ipack, mat);
  data->AddSubset(sub);
}

void AppendGeoHexaHedronData(EntityData* data, const Vector4* vecpos, 
                             const Matrix4& mat) {
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  const int32_t kVertexCount = CalcBoxVertexCount();
  const int32_t kIndexCount = 0;
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  vpack.move(data->vdata()->vertex_count() - kVertexCount);
  Subset sub = AppendGeoHexaHedronSubset(&vpack, vecpos, mat);
  data->AddSubset(sub);
}

void AppendGeoBoxFrameData(EntityData* data, const Matrix4& mat) {
  VertexPack vpack(data->vdata());
  IndexPack ipack(data->idata());
  const int32_t kVertexCount = CalcBoxVertexCount();
  const int32_t kIndexCount = CalcBoxFrameIndexCount();
  data->vdata()->extend(kVertexCount);
  data->idata()->extend(kIndexCount);
  vpack.move(data->vdata()->vertex_count() - kVertexCount);
  ipack.move(data->idata()->count() - kIndexCount);
  Subset sub = AppendGeoBoxFrameSubset(&vpack, &ipack, mat);
  data->AddSubset(sub);
}

EntityDataPtr CreateHexaHedron(VertexDesc* desc, const Vector4* vecpos, 
                               Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoHexaHedronData(data.get(), vecpos, mat);
  return data;
}

EntityDataPtr CreateHexaHedronFrame(VertexDesc* desc, const Vector4* vecpos,
                                    const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoHexaHedronData(data.get(), vecpos, mat);
  return data;
}


EntityDataPtr CreateBox(VertexDesc* desc, const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoBoxData(data.get(), mat);
  return data;
}

EntityDataPtr CreateBoxFrame(VertexDesc* desc, const Matrix4& mat) {
  return CreateHexaHedronFrame(desc, box_position, mat);
}
}  // namespace azer
