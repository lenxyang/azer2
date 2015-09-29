#include "azer/render/util/geometry/indices_util.h"

#include "base/strings/string_util.h"
#include "azer/math/math.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/util/index_pack.h"
#include "azer/render/util/vertex_pack.h"


namespace azer {
#define FUNC_BOOL_RET(func)  {                                          \
    bool ret = (func);                                                  \
    if (!ret) return false;                                             \
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

int32 GetSemanticIndex(const std::string& name, int32 semantic_index, 
                       VertexDesc* d) {
  const VertexDesc::Desc* desc = d->descs();
  for (int32 i = 0; i < d->element_num(); ++i) {
    if (base::strncasecmp(name.c_str(), (desc + i)->name, name.length()) == 0
        && (desc + i)->semantic_index == semantic_index) {
      return i;
    }
  }

  return -1;
}

void CalcNormal(VertexData* vbd, IndicesData* idata) {
  const int32 kPositionIndex = 0;
  const int32 kNormalIndex = GetSemanticIndex("normal", 0, vbd->desc().get());
  if (kNormalIndex == -1)
    return;

  std::vector<float> used;
  std::vector<Vector4> normals;
  used.resize(vbd->vertex_num());
  normals.resize(vbd->vertex_num());
  
  VertexPack vpack(vbd);
  vpack.first();
  for (int i = 0; i < vbd->vertex_num(); ++i) {
    DCHECK(!vpack.end());
    vpack.WriteVector4(Vector4(0.0f, 0.0f, 0.0f, 0.0f), kNormalIndex);
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
    vpack.ReadVector4(&p1, kPositionIndex);
    CHECK(vpack.move(idx2));
    vpack.ReadVector4(&p2, kPositionIndex);
    CHECK(vpack.move(idx3));
    vpack.ReadVector4(&p3, kPositionIndex);
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
    vpack.WriteVector4(normal, kNormalIndex);
    vpack.next(1);
  }
}

void CalcTriangleListNormal(VertexData* vbd, int* indices) {
  const int32 kPositionIndex = 0;
  const int32 kNormalIndex = GetSemanticIndex("normal", 0, vbd->desc().get());
  if (kNormalIndex == -1)
    return;
  std::vector<float> used;
  std::vector<Vector4> normals;
  used.resize(vbd->vertex_num());
  normals.resize(vbd->vertex_num());
  
  VertexPack vpack(vbd);
  vpack.first();
  for (int i = 0; i < vbd->vertex_num(); ++i) {
    vpack.WriteVector4(Vector4(0.0f, 0.0f, 0.0f, 0.0f), kNormalIndex);
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
    vpack.ReadVector4(&p1, kPositionIndex);
    CHECK(vpack.move(idx2));
    vpack.ReadVector4(&p2, kPositionIndex);
    CHECK(vpack.move(idx3));
    vpack.ReadVector4(&p3, kPositionIndex);
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
    vpack.WriteVector4(normal, kNormalIndex);
    vpack.next(1);
  }
}
}  // namespace azer
