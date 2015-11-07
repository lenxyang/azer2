#include "azer/render/geometry/util.h"

#include "base/strings/string_util.h"
#include "azer/math/math.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/index_pack.h"
#include "azer/render/vertex_pack.h"


namespace azer {
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

void GenerateConeHat(bool up, float top, float bottom, float radius, int32 slice, 
                     VertexPack* vpack, IndexPack* ipack) {
  const int begin = vpack->index();
  float slice_degree = 360.0f / slice;
  vpack->WriteVector4(Vector4(0.0f, top, 0.0f, 1.0f), VertexPos(0, 0));
  vpack->next(1);
  for (int32 i = 0; i < slice; ++i) {
    float degree = 360.0f - slice_degree *  i;
    float x = azer::cos(Degree(degree)) * radius;
    float z = azer::sin(Degree(degree)) * radius;

    vpack->WriteVector4(Vector4(x, bottom, z, 1.0f), VertexPos(0, 0));
    vpack->next(1);
  }
  
  const int end = vpack->index();
  for (int i = 0; i < slice; ++i) {
    int index1 = i + begin + 1;
    int index2 = (i + 1) % slice + begin + 1;
    CHECK(ipack->WriteAndAdvance(begin));
    if (up) {
      CHECK(ipack->WriteAndAdvance(index1));
      CHECK(ipack->WriteAndAdvance(index2));
    } else {
      CHECK(ipack->WriteAndAdvance(index2));
      CHECK(ipack->WriteAndAdvance(index1));
    }
  }
}

void GenerateBarrel(float top_radius, float bottom_radius, float height, 
                    int32 stack, int32 slice, VertexPack* vpack, IndexPack* ipack) {
  VertexPos tpos;
  GetSemanticIndex("texcoord", 0, vpack->desc(), &tpos);
  float height_unit = height / ((float)stack - 1.0f);
  float radius_unit = (bottom_radius - top_radius) / (float)stack;
  float slice_radius = top_radius;
  float y = height;
  float tex_u_unit = 1.0f / slice;
  float tex_v_unit = 1.0f / (stack + 2.0f);
  const int32 begin = vpack->index();
  for (int i = 0; i < stack; ++i) {
    slice_radius += radius_unit;
    for (int j = 0; j < slice; ++j) {
      float degree = 360.0f - j * 360.0f / slice;
      float x = slice_radius * cos(Degree(degree));
      float z = slice_radius * sin(Degree(degree));

      vpack->WriteVector4(Vector4(x, y, z, 1.0f), VertexPos(0, 0));
      CHECK(vpack->next(1));
      float u = j * tex_u_unit;
      float v = (i + 1) * tex_v_unit;
      vpack->WriteVector2(Vector2(0.0f, 0.0f), tpos); 
    }
    y -= height_unit;
  }
  
  for (int i = 0; i < stack - 1; ++i) {
    int32 line1 = begin + i * slice; 
    int32 line2 = begin + (i + 1) * slice; 
    for (int j = 0; j < slice; ++j) {
      int index1 = j % slice;
      int index2 = (j + 1) % slice;
      CHECK(ipack->WriteAndAdvance(line1 + index2));
      CHECK(ipack->WriteAndAdvance(line1 + index1));
      CHECK(ipack->WriteAndAdvance(line2 + index1));
      
      CHECK(ipack->WriteAndAdvance(line1 + index2));
      CHECK(ipack->WriteAndAdvance(line2 + index1));
      CHECK(ipack->WriteAndAdvance(line2 + index2));
    }
  }
}

void CalcNormal(SlotVertexData* vbd, IndicesData* idata) {
  VertexPos npos;
  if (!GetSemanticIndex("normal", 0, vbd->desc(), &npos)) {
    return;
  }

  std::vector<float> used;
  std::vector<Vector4> normals;
  used.resize(vbd->vertex_num());
  normals.resize(vbd->vertex_num());
  
  VertexPack vpack(vbd);
  vpack.first();
  for (int i = 0; i < vbd->vertex_num(); ++i) {
    DCHECK(!vpack.end());
    vpack.WriteVector4(Vector4(0.0f, 0.0f, 0.0f, 0.0f), npos);
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
    Vector4 p1, p2, p3;
    CHECK(vpack.move(idx1));
    vpack.ReadVector4(&p1, VertexPos(0, 0));
    CHECK(vpack.move(idx2));
    vpack.ReadVector4(&p2, VertexPos(0, 0));
    CHECK(vpack.move(idx3));
    vpack.ReadVector4(&p3, VertexPos(0, 0));
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
    vpack.WriteVector4(normal, npos);
    vpack.next(1);
  }
}

void CalcTriangleListNormal(SlotVertexData* vbd, int* indices) {
  VertexPos npos;
  if (!GetSemanticIndex("normal", 0, vbd->desc(), &npos)) {
    return;
  }

  std::vector<float> used;
  std::vector<Vector4> normals;
  used.resize(vbd->vertex_num());
  normals.resize(vbd->vertex_num());
  
  VertexPack vpack(vbd);
  vpack.first();
  for (int i = 0; i < vbd->vertex_num(); ++i) {
    vpack.WriteVector4(Vector4(0.0f, 0.0f, 0.0f, 0.0f), npos);
    vpack.next(1);
    used[i] = 0.0f;
    normals[i] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  vpack.first();
  for (int i = 0; i < vbd->vertex_num(); i+=3) {
    int idx1 = *(indices + i);
    int idx2 = *(indices + i + 1);
    int idx3 = *(indices + i + 2);
    Vector4 p1, p2, p3;
    CHECK(vpack.move(idx1));
    vpack.ReadVector4(&p1, VertexPos(0, 0));
    CHECK(vpack.move(idx2));
    vpack.ReadVector4(&p2, VertexPos(0, 0));
    CHECK(vpack.move(idx3));
    vpack.ReadVector4(&p3, VertexPos(0, 0));
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
    vpack.WriteVector4(normal, npos);
    vpack.next(1);
  }
}
}  // namespace azer
