#include "azer/render/geometry.h"

#include "azer/math/math.h"
#include "azer/render/mesh.h"
#include "azer/render/index_pack.h"
#include "azer/render/bounding_volumn.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/render_system.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

// =====================================================================
// Code for Generate Sphere Objects
namespace {

#define FUNC_BOOL_RET(func)  {                  \
    bool ret = (func);                          \
    if (!ret) return false;                     \
  }

bool GenerateTopTriangleStrip(int32 top, int32 begin, int32 vertex_num,
                              bool closed, IndexPack* ipack) {
  int num = closed ? vertex_num : vertex_num - 1;
  for (int i = 0; i < num; ++i) {
    int index1 = i % vertex_num + begin;
    int index2 = (i + 1) % vertex_num + begin;
    FUNC_BOOL_RET(ipack->WriteAndAdvance(top));
    FUNC_BOOL_RET(ipack->WriteAndAdvance(index1));
    FUNC_BOOL_RET(ipack->WriteAndAdvance(index2));
  }
  return true;
}

bool GenerateBottomTriangleStrip(int32 bottom, int32 begin, int32 vertex_num,
                                 bool closed, IndexPack* ipack) {
  int num = closed ? vertex_num : vertex_num - 1;
  for (int i = 0; i < num; ++i) {
    int index1 = i % vertex_num + begin;
    int index2 = (i + 1) % vertex_num + begin;
    FUNC_BOOL_RET(ipack->WriteAndAdvance(bottom));
    FUNC_BOOL_RET(ipack->WriteAndAdvance(index2));
    FUNC_BOOL_RET(ipack->WriteAndAdvance(index1));
  }
  return true;
}

bool GenerateStripIndex(int32 line1, int32 line2, int32 vertex_num, bool closed,
                        IndexPack* ipack) {
  int num = closed ? vertex_num : vertex_num - 1;
  for (int i = 0; i < num; ++i) {
    int index1 = i % vertex_num;
    int index2 = (i + 1) % vertex_num;
    FUNC_BOOL_RET(ipack->WriteAndAdvance(line1 + index2));
    FUNC_BOOL_RET(ipack->WriteAndAdvance(line1 + index1));
    FUNC_BOOL_RET(ipack->WriteAndAdvance(line2 + index1));

    FUNC_BOOL_RET(ipack->WriteAndAdvance(line1 + index2));
    FUNC_BOOL_RET(ipack->WriteAndAdvance(line2 + index1));
    FUNC_BOOL_RET(ipack->WriteAndAdvance(line2 + index2));
  }
  return true;
}

void MergeMeshPart(MeshPart* merge_to, MeshPart* part) {
  for (int32 i = 0; i < part->entity_count(); ++i) {
    merge_to->AddEntity(part->entity_at(i));
  }
}

inline int32 CalcSphereIndexNum(int32 stack_num, int32 slice_num) {
  return (stack_num - 2 - 1) * slice_num * 3 * 2 + slice_num * 2 * 3;
}

inline int32 CalcSphereVertexNum(int32 stack_num, int32 slice_num) {
  return (stack_num - 2) * slice_num + 2;
}

VertexDataPtr InitSphereVertexData(VertexDesc* desc, const Matrix4& matrix,
                                   float radius, int32 stack, int32 slice) {
  VertexPos texpos;
  bool hastex = GetSemanticIndex("texcoord", 0, desc, &texpos);

  const int32 kVertexNum = CalcSphereVertexNum(stack, slice);
  VertexDataPtr vdata(new VertexData(desc, kVertexNum));
  VertexPack vpack(vdata);

  int num = 0;
  CHECK(vpack.first());
  vpack.WriteVector3Or4(matrix * Vector4(0.0f, radius, 0.0f, 1.0f), VertexPos(0, 0));
  if (hastex) {
    vpack.WriteVector2(Vector2(0.0f, 0.0f), texpos);
  }
  num++;

  for (int i = 1; i < stack - 1; ++i) {
    float y = radius * sin(Degree(90.0f - i * 180.0f / (float)stack));
    float slice_radius =  cos(Degree(90.0f - i * 180.0f / (float)stack));
    for (int j = 0; j < slice; ++j) {
      float degree = 360.0f - j * 360.0f / slice;
      float x = radius * slice_radius * cos(Degree(degree));
      float z = radius * slice_radius * sin(Degree(degree));

      if (hastex) {
        float tu = j * 1.0f / slice;
        float tv = i * 1.0f / (stack - 1);
        vpack.WriteVector2(Vector2(tu, tv), texpos);
      }

      CHECK(vpack.next(1));
      Vector4 pos = std::move(matrix * Vector4(x, y, z, 1.0f));
      vpack.WriteVector3Or4(pos, VertexPos(0, 0));
      num++;
    }
  }

  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(matrix * Vector4(0.0f, -radius, 0.0f, 1.0f), VertexPos(0, 0));
  if (hastex) {
    vpack.WriteVector2(Vector2(1.0f, 1.0f), texpos);
  }
  num++;
  DCHECK_EQ(num, kVertexNum);
  return vdata;
}

IndicesDataPtr InitSphereIndicesData(int32 stack, int32 slice) {
  const int kIndexNum = CalcSphereIndexNum(stack, slice);
  const int32 kVertexNum = CalcSphereVertexNum(stack, slice);
  int bottom_index = kVertexNum - 1;
  IndicesDataPtr idata(new IndicesData(kIndexNum));  
  IndexPack ipack(idata);
  CHECK(GenerateTopTriangleStrip(0, 1, slice, true, &ipack));
  for (int i = 1; i < stack - 2; ++i) {
    CHECK(GenerateStripIndex(1 + slice * (i - 1), 1 + slice * i, slice, true,
                             &ipack));
  }
  CHECK(GenerateBottomTriangleStrip(bottom_index, bottom_index - slice,
                                    slice, true, &ipack));
  CHECK(ipack.count() == kIndexNum);
  return idata;
}

IndicesDataPtr InitSphereWireFrameIndicesData(int32 stack, int32 slice) {
  const int kIndexNum = (stack - 1) * slice * 2 + (stack - 2) * (slice + 1) * 2;
  const int32 kVertexNum = CalcSphereVertexNum(stack, slice);
  IndicesDataPtr idata(new IndicesData(kIndexNum));  
  IndexPack ipack(idata);
  for (int i = 0; i < slice; ++i) {
    CHECK(ipack.WriteAndAdvance(0));
    CHECK(ipack.WriteAndAdvance(1 + i));
  }

  for (int i = 1; i < stack - 2; ++i) {
    for (int j = 0; j < slice; ++j) {
      CHECK(ipack.WriteAndAdvance(1 + (i - 1) * slice + j));
      CHECK(ipack.WriteAndAdvance(1 + i * slice + j));
    }
  }

  for (int i = 0; i < slice; ++i) {
    CHECK(ipack.WriteAndAdvance(kVertexNum - 1));
    CHECK(ipack.WriteAndAdvance(kVertexNum - 1 - (i + 1)));
  }

  for (int i = 1; i < stack - 1; ++i) {
    for (int j = 0; j < slice; ++j) {
      CHECK(ipack.WriteAndAdvance(1 + (i - 1) * slice + j));
      CHECK(ipack.WriteAndAdvance(1 + (i - 1) * slice + (j + 1) % slice));
    }
  }
  CHECK(ipack.count()  == kIndexNum);
  return idata;
}
}  // namespace

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
  for (int i = 0; i < idata->num(); i+=3) {
    uint32 idx1 = ipack.ReadAndAdvanceOrDie();
    uint32 idx2 = ipack.ReadAndAdvanceOrDie();
    uint32 idx3 = ipack.ReadAndAdvanceOrDie();
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
  for (int i = 0; i < normals.size(); ++i) {
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

  for (int i = 0; i < normals.size(); ++i) {
    normals[i] /= used[i];
  }

  vpack.first();
  for (int i = 0; i < normals.size(); ++i) {
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
  
  for (int32 i = 0; i < ipack.count(); i+=3) {
    uint32 index[3];
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


// class Sphere objects
EntityPtr CreateSphereEntity(VertexDesc* desc,const GeoSphereParams& params,
                             const Matrix4& mat) {
  VertexDataPtr vdata(InitSphereVertexData(desc, mat, params.radius,
                                           params.stack, params.slice));
  IndicesDataPtr idata = InitSphereIndicesData(params.stack, params.slice);

  VertexPos npos;
  if (GetSemanticIndex("normal", 0, desc, &npos)) {
    CalcIndexedTriangleNormal(vdata, idata);
  }

  RenderSystem* rs = RenderSystem::Current();
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), idata);
  EntityPtr entity(new Entity(vb, ib));
  Vector4 vmin = mat * Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
  Vector4 vmax = mat * Vector4( 0.5f,  0.5f,  0.5f, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  entity->AddSubset(Subset(0, vb->vertex_count(), 0, ib->indices_count()));
  return entity;
}


EntityPtr CreateSphereFrameEntity(VertexDesc* desc, const GeoSphereParams& params,
                                  const Matrix4& mat) {
  VertexDataPtr vdata(InitSphereVertexData(desc, mat, params.radius,
                                           params.stack, params.slice));
  IndicesDataPtr idata = InitSphereIndicesData(params.stack, params.slice);
  IndicesDataPtr iedata = InitSphereWireFrameIndicesData(params.stack, params.slice);
  VertexPos npos;
  if (GetSemanticIndex("normal", 0, desc, &npos)) {
    CalcIndexedTriangleNormal(vdata, idata);
  }
  RenderSystem* rs = RenderSystem::Current();
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), iedata);
  EntityPtr entity(new Entity(vb, ib));
  Vector4 vmin = mat * Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
  Vector4 vmax = mat * Vector4( 0.5f,  0.5f,  0.5f, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  Subset subset(0, vb->vertex_count(), 0, ib->indices_count());
  subset.primitive = kLineList;
  entity->AddSubset(subset);
  return entity;
}

MeshPartPtr CreateSphereMeshPart(Effect* effect, const GeoSphereParams& params,
                                 const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(effect));
  part->AddEntity(CreateSphereEntity(effect->vertex_desc(), params, mat));
  return part;
}

MeshPartPtr CreateSphereFrameMeshPart(Effect* e, const GeoSphereParams& params,
                                      const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateSphereFrameEntity(e->vertex_desc(), params, mat));
  return part;
}

// Generate Box Geometry 
namespace {
VertexDataPtr CreateBoxVertexData(VertexDesc* desc) {
  const Vector4 position[] = {
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

  Vector4 normal[] = {
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
  VertexPos normal_pos, tex0_pos;
  bool kHasNormal0Idx = GetSemanticIndex("normal", 0, desc, &normal_pos);
  bool kHasTexcoord0Idx = GetSemanticIndex("texcoord", 0, desc, &tex0_pos);
  VertexDataPtr vdata(new VertexData(desc, arraysize(indices)));
  VertexPack vpack(vdata);
  vpack.first();
  for (int i = 0; i < static_cast<int>(arraysize(indices)); ++i) {
    int index = indices[i];
    DCHECK(!vpack.end());
    vpack.WriteVector3Or4(position[index], VertexPos(0, 0));
    vpack.WriteVector2(texcoord0[index], tex0_pos);
    vpack.next(1);
  }
  DCHECK(vpack.end());

  if (kHasNormal0Idx) {
    vpack.first(); 
    for (int i = 0; i < static_cast<int>(arraysize(indices)); i += 6) {
      int index = i / arraysize(normal);
      for (int j = 0; j < 6; ++j) { 
        vpack.WriteVector3Or4(normal[index], normal_pos);
        vpack.next(1);
      }
    }
  }

  return vdata;
}

IndicesDataPtr CreateBoxFrameIndicesData() {
  int32 edge_indices[] = {0, 2, 2, 1, 1, 4, 4, 0,
                          0, 14, 2, 8, 1, 7, 4, 13,
                          14, 8, 8, 7, 7, 13, 13, 14};
  IndicesDataPtr idata(new IndicesData(arraysize(edge_indices)));
  IndexPack ipack(idata);
  for (uint32 i = 0; i < arraysize(edge_indices); ++i) {
    CHECK(ipack.WriteAndAdvance(edge_indices[i]));
  }
  return idata;
}
}

EntityPtr CreateBoxEntity(VertexDesc* desc, const Matrix4& mat) {
  RenderSystem* rs = RenderSystem::Current();
  VertexDataPtr vdata = CreateBoxVertexData(desc);;
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  EntityPtr entity(new Entity(vb));
  Vector4 vmin = mat * Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
  Vector4 vmax = mat * Vector4( 0.5f,  0.5f,  0.5f, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  entity->AddSubset(Subset(0, vb->vertex_count(), 0, 0));
  return entity;
}

EntityPtr CreateBoxFrameEntity(VertexDesc* desc, const Matrix4& mat) {
  RenderSystem* rs = RenderSystem::Current();
  VertexDataPtr vdata = CreateBoxVertexData(desc);;
  IndicesDataPtr idata = CreateBoxFrameIndicesData();
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), idata);
  EntityPtr entity(new Entity(vb, ib));
  Vector4 vmin = mat * Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
  Vector4 vmax = mat * Vector4( 0.5f,  0.5f,  0.5f, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  Subset subset(0, vb->vertex_count(), 0, ib->indices_count());
  subset.primitive = kLineList;
  entity->AddSubset(subset);
  return entity;
}

MeshPartPtr CreateBoxMeshPart(Effect* e, const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateBoxEntity(e->vertex_desc(), mat));
  return part;  
}

MeshPartPtr CreateBoxFrameMeshPart(Effect* e, const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateBoxFrameEntity(e->vertex_desc(), mat));
  return part;  
}

// Plane 
namespace {
VertexDataPtr CreatePlaneVertexData(VertexDesc* desc, 
                                    const GeoPlaneParams& params,
                                    const Matrix4& matrix) {
  VertexPos npos, tpos;
  const bool kHasNormalIndex = GetSemanticIndex("normal", 0, desc, &npos);
  const bool kHasTexcoordIndex = GetSemanticIndex("texcoord", 0, desc, &tpos);

  int32 kVertexCount = (params.row + 1) * (params.column + 1);
  VertexDataPtr vdata(new VertexData(desc, kVertexCount));
  VertexPack vpack(vdata);
  vpack.first();
  
  int beginx = -params.column * params.column_width * 0.5;
  int beginz = -params.row * params.row_width * 0.5;
  for (int i = 0; i < params.row + 1; ++i) {
    for (int j = 0; j < params.column + 1; ++j) {
      float x = beginx + j * params.column_width;
      float z = beginz + i * params.row_width;
      vpack.WriteVector3Or4(matrix * Vector4(x,    0.0f, z, 1.0f), VertexPos(0, 0));
      vpack.WriteVector3Or4(matrix * Vector4(0.0f, 1.0f, 0.0f, 0.0f), npos);

      float tu = (float)j / params.column;
      float tv = (float)i / params.row;
      vpack.WriteVector2(Vector2(tu, tv), tpos);
      vpack.next(1);
    }
  }
  return vdata;
}

IndicesDataPtr CreatePlaneIndicesData(const GeoPlaneParams& params) {
  const int32 kIndexNum = params.row * params.column * 2 * 3;
  IndicesDataPtr idata(new IndicesData(kIndexNum));
  IndexPack ipack(idata);
  for (int i = 0; i < params.row; ++i) {
    for (int j = 0; j < params.column; ++j) {
      int cur_line = i * (params.column + 1);
      int next_line = (i + 1) * (params.column + 1);
      CHECK(ipack.WriteAndAdvance(cur_line  + j));
      CHECK(ipack.WriteAndAdvance(next_line + j));
      CHECK(ipack.WriteAndAdvance(next_line + j + 1));
      CHECK(ipack.WriteAndAdvance(cur_line  + j));
      CHECK(ipack.WriteAndAdvance(next_line + j + 1));
      CHECK(ipack.WriteAndAdvance(cur_line  + j + 1));
    }
  }
  return idata;
}

IndicesDataPtr CreatePlaneFrameIndicesData(const GeoPlaneParams& params) {
  int32 count = (params.row + 1) * 2 + (params.column + 1) * 2;
  IndicesDataPtr idata(new IndicesData(count));
  IndexPack ipack(idata);
  for (uint32 i = 0; i < params.row; ++i) {
    int32 index1 = i * (params.column + 1);
    int32 index2 = (i  + 1) * (params.column + 1) - 1;
    CHECK(ipack.WriteAndAdvance(index1));
    CHECK(ipack.WriteAndAdvance(index2));
  }

  for (uint32 i = 0; i < params.column; ++i) {
    int32 index1 = i;
    int32 index2 = (params.row + 1) * params.column + i;
    CHECK(ipack.WriteAndAdvance(index1));
    CHECK(ipack.WriteAndAdvance(index2));
  }
  return idata;
}
}

EntityPtr CreatePlaneEntity(VertexDesc* desc, const GeoPlaneParams& params,
                            const Matrix4& mat) {
  RenderSystem* rs = RenderSystem::Current();
  VertexDataPtr vdata = CreatePlaneVertexData(desc, params, mat);;
  IndicesDataPtr idata = CreatePlaneIndicesData(params);
  CalcIndexedTriangleListTangentAndBinormal(vdata, idata);
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), idata);
  EntityPtr entity(new Entity(vb, ib));
  Vector4 vmin = mat * Vector4(-params.column_width * params.column * 0.5f,  0.00f, 
                               -params.row_width * params.row * 0.5f, 1.0f);
  Vector4 vmax = mat * Vector4( params.column_width * params.column * 0.5f,  0.01f,  
                                params.row_width * params.row * 0.5f, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  entity->AddSubset(Subset(0, vb->vertex_count(), 0, ib->indices_count()));
  return entity;
}

EntityPtr CreatePlaneFrameEntity(VertexDesc* desc, const GeoPlaneParams& params,
                                 const Matrix4& mat) {
  RenderSystem* rs = RenderSystem::Current();
  VertexDataPtr vdata = CreatePlaneVertexData(desc, params, mat);
  IndicesDataPtr idata = CreatePlaneFrameIndicesData(params);
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), idata);
  EntityPtr entity(new Entity(vb, ib));
  Vector4 vmin = mat * Vector4(-params.column_width * 0.5f,  0.00f, 
                               -params.row_width * 0.5f, 1.0f);
  Vector4 vmax = mat * Vector4( params.column_width * 0.5f,  0.01f,  
                                params.row_width * 0.5f, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  Subset subset(0, vb->vertex_count(), 0, ib->indices_count());
  subset.primitive = kLineList;
  entity->AddSubset(subset);
  return entity;
}

MeshPartPtr CreatePlaneMeshPart(Effect* e, const GeoPlaneParams& params,
                                const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreatePlaneEntity(e->vertex_desc(), params, mat));
  return part;  
}

MeshPartPtr CreatePlaneFrameMeshPart(Effect* e,const GeoPlaneParams& params,
                                     const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreatePlaneFrameEntity(e->vertex_desc(), params, mat));
  return part;  
}

// class round
namespace {
VertexDataPtr CreateRoundVertexData(VertexDesc* desc, const Matrix4& mat,
                                    float radius, float slice) {
  VertexPos npos;
  GetSemanticIndex("normal", 0, desc, &npos);
  const int32 kVertexNum = 1 + slice + 1;
  float degree = 360.0f / (float)slice;
  VertexDataPtr vdata(new VertexData(desc, kVertexNum));
  VertexPack vpack(vdata);
  vpack.first();
  vpack.WriteVector3Or4(mat * Vector4(0, 0, 0, 1.0f), VertexPos(0, 0));
  vpack.WriteVector3Or4(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f), npos);
  vpack.next(1);
  for (int i = 1; i < kVertexNum; ++i) {
    float x = cos(Degree(i * degree)) * radius;
    float z = sin(Degree(i * degree)) * radius;
    vpack.WriteVector3Or4(mat * Vector4(x, 0, z, 1.0f), VertexPos(0, 0));
    vpack.WriteVector3Or4(mat * Vector4(0.0f, 1.0f, 0.0f, 0.0f), npos);
    vpack.next(1);
  }
  CHECK(vpack.end());
  return vdata;
}

IndicesDataPtr CreateRoundInidcesData(int32 slice) {
  IndicesDataPtr idata(new IndicesData(slice * 3));  
  IndexPack ipack(idata);
  for (int i = 0; i < slice; ++i) {
    int index1 = 1 + (i + 1) % slice;
    int index2 = 1 + i;
    CHECK(ipack.WriteAndAdvance(0));
    CHECK(ipack.WriteAndAdvance(index1));
    CHECK(ipack.WriteAndAdvance(index2));
  }
  return idata;
}

IndicesDataPtr CreateCircleInidcesData(int32 slice) {
  const int kIndexNum = slice * 2;
  IndicesDataPtr idata(new IndicesData(kIndexNum));  
  IndexPack ipack(idata);
  for (int i = 0; i < slice; ++i) {
    CHECK(ipack.WriteAndAdvance(i + 1));
    CHECK(ipack.WriteAndAdvance((i + 1) % slice + 1));
  }
  return idata;
}
} // namespace

EntityPtr CreateRoundEntity(VertexDesc* desc, float radius, int32 slice, 
                            const Matrix4& mat) {
  VertexDataPtr vdata = CreateRoundVertexData(desc, mat, radius, slice);
  IndicesDataPtr idata = CreateRoundInidcesData(slice);
  CalcIndexedTriangleListTangentAndBinormal(vdata, idata);
  RenderSystem* rs = RenderSystem::Current();
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), idata);
  EntityPtr entity(new Entity(vb, ib));
  Vector4 vmin = mat * Vector4(-radius, -0.00f, -radius, 1.0f);
  Vector4 vmax = mat * Vector4( radius,  0.01f,  radius, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  entity->AddSubset(Subset(0, vb->vertex_count(), 0, ib->indices_count()));
  return entity;
}

EntityPtr CreateCircleEntity(VertexDesc* desc, float radius, int32 slice, 
                             const Matrix4& mat) {
  VertexDataPtr vdata = CreateRoundVertexData(desc, mat, radius, slice);
  IndicesDataPtr idata = CreateCircleInidcesData(slice);
  RenderSystem* rs = RenderSystem::Current();
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), idata);
  EntityPtr entity(new Entity(vb, ib));
  Vector4 vmin = mat * Vector4(-radius, -0.00f, -radius, 1.0f);
  Vector4 vmax = mat * Vector4( radius,  0.01f,  radius, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  Subset subset(0, vb->vertex_count(), 0, ib->indices_count());
  subset.primitive = kLineList;
  entity->AddSubset(subset);
  return entity;
}
MeshPartPtr CreateRoundMeshPart(Effect* e, float radius, int slice, 
                                const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateRoundEntity(e->vertex_desc(), radius, slice, mat));
  return part;  
}

MeshPartPtr CreateCircleMeshPart(Effect* e, float radius, int slice,
                                 const Matrix4& mat) {
  
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateCircleEntity(e->vertex_desc(), radius, slice, mat));
  return part;  
}

// cone
EntityPtr CreateTaperEntity(VertexDesc* desc, const GeoConeParams& params, 
                            const Matrix4& mat) {
  const int32 kVertexNum = 1 + params.slice + 1;
  float degree = 360.0f / (float)params.slice;
  VertexDataPtr vdata(new VertexData(desc, kVertexNum));
  VertexPack vpack(vdata);
  vpack.first();
  vpack.WriteVector3Or4(mat * Vector4(0, params.height, 0, 1.0f), 
                        VertexPos(0, 0));
  vpack.next(1);
  for (int i = 1; i < kVertexNum; ++i) {
    float x = cos(Degree(i * degree)) * params.radius;
    float z = sin(Degree(i * degree)) * params.radius;
    vpack.WriteVector3Or4(mat * Vector4(x, 0, z, 1.0f), VertexPos(0, 0));
    vpack.next(1);
  }
  CHECK(vpack.end());

  IndicesDataPtr idata = CreateRoundInidcesData(params.slice);
  CalcIndexedTriangleNormal(vdata, idata);
  CalcIndexedTriangleListTangentAndBinormal(vdata, idata);

  RenderSystem* rs = RenderSystem::Current();
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), idata);
  EntityPtr entity(new Entity(vb, ib));
  Vector4 vmin = mat * Vector4(-params.radius, 0.0f, -params.radius, 1.0f);
  Vector4 vmax = mat * Vector4( params.radius, params.height,  
                                params.radius, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  entity->AddSubset(Subset(0, vb->vertex_count(), 0, ib->indices_count()));
  return entity;
}
MeshPartPtr CreateTaperMeshPart(Effect* e, const GeoConeParams& params,
                                const Matrix4& mat) {
  
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateTaperEntity(e->vertex_desc(), params, mat));
  return part;  
}

MeshPartPtr CreateConeMeshPart(Effect* e, const GeoConeParams& params, 
                               const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  EntityPtr taper = CreateTaperEntity(e->vertex_desc(), params, mat);

  Matrix4 round_mat = std::move(mat * RotateX(Degree(180.0)));
  EntityPtr round = CreateRoundEntity(e->vertex_desc(), params.radius, params.slice,
                                      round_mat);
  
  part->AddEntity(round);
  part->AddEntity(taper);
  return part;
}

namespace {
int32 CalcCylinderIndexNum(int32 stack_num, int32 slice_num) {
  return (stack_num - 1) * slice_num * 3 * 2;
}

int32 CalcCylinderVertexNum(int32 stack_num, int32 slice_num) {
  return stack_num * slice_num;
}
}

EntityPtr CreateBarrelEntity(VertexDesc* desc, const GeoBarrelParams& params, 
                             const Matrix4& mat) {
  const int kVertexNum = CalcCylinderVertexNum(params.stack, params.slice);
  const int kIndexNum = CalcCylinderIndexNum(params.stack, params.slice);
  VertexDataPtr vdata(new VertexData(desc, kVertexNum));
  IndicesDataPtr idata(new IndicesData(kIndexNum));  
  VertexPack vpack(vdata);
  VertexPos tpos;
  GetSemanticIndex("texcoord", 0, desc, &tpos);
  float height_unit = params.height / ((float)params.stack - 1.0f);
  float radius_unit = (params.bottom_radius - params.top_radius)
      / (float)params.stack;
  float slice_radius = params.top_radius;
  float y = params.height;
  float tex_u_unit = 1.0f / params.slice;
  float tex_v_unit = 1.0f / (params.stack + 2.0f);
  vpack.first();
  for (int i = 0; i < params.stack; ++i) {
    for (int j = 0; j < params.slice; ++j) {
      float degree = 360.0f - j * 360.0f / params.slice;
      float x = slice_radius * cos(Degree(degree));
      float z = slice_radius * sin(Degree(degree));

      vpack.WriteVector3Or4(mat * Vector4(x, y, z, 1.0f), VertexPos(0, 0));
      float u = j * tex_u_unit;
      float v = (i + 1) * tex_v_unit;
      vpack.WriteVector2(Vector2(0.0f, 0.0f), tpos); 
      vpack.next(1);
    }
    slice_radius += radius_unit;
    y -= height_unit;
  }

  IndexPack ipack(idata);
  for (int i = 0; i < params.stack - 1; ++i) {
    int32 line1 = i * params.slice; 
    int32 line2 = (i + 1) * params.slice; 
    for (int j = 0; j < params.slice; ++j) {
      int index1 = j % params.slice;
      int index2 = (j + 1) % params.slice;
      CHECK(ipack.WriteAndAdvance(line1 + index2));
      CHECK(ipack.WriteAndAdvance(line1 + index1));
      CHECK(ipack.WriteAndAdvance(line2 + index1));
      
      CHECK(ipack.WriteAndAdvance(line1 + index2));
      CHECK(ipack.WriteAndAdvance(line2 + index1));
      CHECK(ipack.WriteAndAdvance(line2 + index2));
    }
  }

  CalcIndexedTriangleNormal(vdata, idata);

  RenderSystem* rs = RenderSystem::Current();
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  IndicesBufferPtr ib = rs->CreateIndicesBuffer(kIndicesBufferOpt(), idata);
  EntityPtr entity(new Entity(vb, ib));
  float rad = std::max(params.top_radius, params.bottom_radius);
  Vector4 vmin = mat * Vector4(-rad,  0.0f,          -rad, 1.0f);
  Vector4 vmax = mat * Vector4( rad,  params.height,  rad, 1.0f);
  entity->set_vmin(Vector3(vmin.x, vmin.y, vmin.z));
  entity->set_vmax(Vector3(vmax.x, vmax.y, vmax.z));
  entity->AddSubset(Subset(0, vb->vertex_count(), 0, ib->indices_count()));
  return entity;
}

MeshPartPtr CreateBarrelMeshPart(Effect* e, const GeoBarrelParams& params, 
                                 const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateBarrelEntity(e->vertex_desc(), params, mat));
  return part;  
}

MeshPartPtr CreateCylinderMeshPart(Effect* e, const GeoBarrelParams& params,
                                   const Matrix4& matrix) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateBarrelEntity(e->vertex_desc(), params, matrix));
  {
    Matrix4 round_matrix = std::move(matrix * RotateX(Degree(180.0)));
    EntityPtr bot = CreateRoundEntity(e->vertex_desc(), params.bottom_radius, 
                                      params.slice, round_matrix);
    part->AddEntity(bot);
  }

  {
    Matrix4 round_matrix = matrix * Translate(Vector3(0.0f, params.height, 0.0f));
    EntityPtr top = CreateRoundEntity(e->vertex_desc(), params.top_radius, 
                                      params.slice, round_matrix);
    part->AddEntity(top);
  }
  return part;
}

EntityPtr CreateTourEntity(
    VertexDesc* desc, const GeoTourParams& params, const Matrix4& mat) {
  return EntityPtr();
}

MeshPartPtr CreateTourMeshPart(Effect* e, const GeoTourParams& params,
                               const Matrix4& mat) {
  MeshPartPtr part(new MeshPart(e));
  part->AddEntity(CreateTourEntity(e->vertex_desc(), params, mat));
  return part;
}

// Axis
MeshPartPtr CreateAxisMeshPart(Effect* e, const GeoAxisParams& params,
                               const Matrix4& matrix) {
  GeoConeParams cone_params;
  cone_params.radius = params.cone_radius;
  cone_params.height = params.cone_height;
  cone_params.slice = params.slice;
  Matrix4 mat = matrix * Translate(Vector3(0.0f, params.axis_length, 0.0f));
  MeshPartPtr part = CreateConeMeshPart(e, cone_params, mat);

  GeoBarrelParams col_params;
  col_params.top_radius = params.axis_radius;
  col_params.bottom_radius = params.axis_radius;
  col_params.slice = params.slice;
  col_params.stack = 12;
  col_params.height = params.axis_length;
  MeshPartPtr colpart = CreateCylinderMeshPart(e, col_params, matrix);
  MergeMeshPart(part, colpart);
  return part;
}

MeshPartPtr CreateLineAxisMeshPart(Effect* e, const GeoAxisParams& params,
                                   const Matrix4& matrix) {
  GeoConeParams cone_params;
  cone_params.radius = params.cone_radius;
  cone_params.height = params.cone_height;
  cone_params.slice = 32;
  Matrix4 mat = matrix * Translate(Vector3(0.0f, params.axis_length, 0.0f));
  MeshPartPtr part = CreateConeMeshPart(e, cone_params, mat);

  Vector3 points[] = {
    Vector3(0.0f, 0.0f, 0.0f),
    Vector3(0.0f, params.axis_length, 0.0f),
  };

  EntityPtr entity = CreateGeoPointsList(
      points, (int)arraysize(points), e->vertex_desc(), matrix);
  entity->set_vmin(points[0]);
  entity->set_vmax(points[1] + Vector3(0.0f, 0.0f, 0.01f));
  Subset subset(0, entity->vertex_buffer_at(0)->vertex_count(), 0, 0);
  subset.primitive = kLineList;
  entity->AddSubset(subset);
  part->AddEntity(entity);
  return part;
}

EntityPtr CreateGeoPointsList(const Vector3* points, int32 count,
                              VertexDesc* desc, const Matrix4& mat) {
  VertexDataPtr vdata(new VertexData(desc, count));
  VertexPack vpack(vdata);
  vpack.first();
  Vector3 vmin(mat * Vector4(points[0], 1.0));
  Vector3 vmax(mat * Vector4(points[0], 1.0));
  for (int32 i = 0; i < count; ++i) {
    Vector4 vec(mat * Vector4(points[i], 1.0f));
    vpack.WriteVector3Or4(vec, VertexPos(0, 0));
    UpdateVMinAndVMax(Vector3(vec), &vmin, &vmax);
    vpack.next(1);
  }

  RenderSystem* rs = RenderSystem::Current();
  VertexBufferGroupPtr vb = CreateVertexBufferGroup(kVertexBufferOpt(), vdata);
  EntityPtr entity(new Entity(vb));
  entity->set_vmin(vmin);
  entity->set_vmax(vmax);
  entity->AddSubset(Subset(0, vb->vertex_count(), 0, 0));
  return entity;
}

}  // namespace azer
