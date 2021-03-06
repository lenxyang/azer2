#include "azer/resource/mesh_load_util.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/anim.h"
#include "azer/render/util.h"
#include "azer/azer.h"
#include "azer/render/bounding_volumn.h"

namespace azer {

namespace {
struct MeshData {
  VertexDataPtr vdata;
  IndicesDataPtr idata;
  Vector3 vmin;
  Vector3 vmax;
};

void CalcTriangleListTangentAndBinormal(VertexData* vd) {
  const VertexDesc* desc = vd->vertex_desc();
  VertexPack pickle(vd);
  VertexPos ppos, npos, tpos, binpos, tagentpos;
  bool has_pos = GetSemanticIndex("position", 0, desc, &ppos);
  bool has_normal = GetSemanticIndex("normal", 0, desc, &npos);
  bool has_tex = GetSemanticIndex("texcoord", 0, desc, &tpos);
  bool has_binormal = GetSemanticIndex("binormal", 0, desc, &binpos);
  bool has_tagent = GetSemanticIndex("tangent", 0, desc, &tagentpos);
  if (!has_pos ||  !has_normal || !has_tex || !has_binormal || !has_tagent) {
    return;
  }

  pickle.first();
  for (int i = 0; i < vd->vertex_count(); i+=3) {
    Vector3 p1, p2, p3;
    Vector2 t1, t2, t3;
    Vector3 tangent, binormal, normal;
    pickle.move(i);
    pickle.ReadVector3Or4(&p1, ppos);
    pickle.ReadVector2(&t1, tpos);
    pickle.move(i + 1);
    pickle.ReadVector3Or4(&p2, ppos);
    pickle.ReadVector2(&t2, tpos);
    pickle.move(i + 1);
    pickle.ReadVector3Or4(&p3, ppos);
    pickle.ReadVector2(&t3, tpos);

    CalcTriangleTBN(p1, t1, p2, t2, p3, t3, &tangent, &normal, &binormal);
    for (int j = 0; j < 3; ++j) {
      pickle.move(i + j);
      pickle.WriteVector3Or4(Vector4(binormal, 0.0f), binpos);
      pickle.WriteVector3Or4(Vector4(tangent, 0.0f), tagentpos);
      pickle.WriteVector3Or4(Vector4(normal, 0.0f), npos);
    }
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
  bool has_binormal = GetSemanticIndex("binormal", 0, desc, &binpos);
  bool has_tagent = GetSemanticIndex("tangent", 0, desc, &tagentpos);
  if (!has_pos ||  !has_normal || !has_tex || !has_binormal || !has_tagent) {
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
      pickle.WriteVector3Or4(Vector4(binormal, 0.0f), binpos);
      pickle.WriteVector3Or4(Vector4(tangent, 0.0f), tagentpos);
      pickle.WriteVector3Or4(Vector4(normal, 0.0f), npos);
    }
  }
}

MeshData LoadMeshData(const aiMesh* paiMesh, VertexDesc* desc) {
  VertexDataPtr vdata(new VertexData(desc, paiMesh->mNumVertices));
  IndicesDataPtr idata(new IndicesData(paiMesh->mNumFaces * 3));
  VertexPack vpack(vdata.get());
  Vector3 vmin  = Vector3(999999.9f, 999999.9f, 999999.9f);
  Vector3 vmax = Vector3(-999999.9f, -999999.9f, -999999.9f);
  VertexPos npos, tpos;
  GetSemanticIndex("normal", 0, desc, &npos);
  GetSemanticIndex("texcoord", 0, desc, &tpos);

  const aiVector3D zero3d(0.0f, 0.0f, 0.0f);
  CHECK(vpack.first());
  for (uint32_t i = 0; i < paiMesh->mNumVertices; ++i) {
    const aiVector3D& aipos = paiMesh->mVertices[i];
    const aiVector3D& normal = paiMesh->mNormals[i];
    const aiVector3D& texcoord =
        paiMesh->HasTextureCoords(0) ? (paiMesh->mTextureCoords[0][i]) : zero3d;

    Vector3 pos(aipos.x, aipos.y, aipos.z);
    UpdateVMinAndVMax(pos, &vmin, &vmax);
    vpack.WriteVector3Or4(Vector4(pos, 1.0f), VertexPos(0, 0));
    vpack.WriteVector2(Vector2(texcoord.x, texcoord.y), tpos);
    vpack.WriteVector3Or4(Vector4(normal.x, normal.y, normal.z, 0.0), npos);
    vpack.next(1);
  }

  IndexPack ipack(idata.get());
  for (uint32_t i = 0; i < paiMesh->mNumFaces; ++i) {
    const aiFace& face = paiMesh->mFaces[i];
    CHECK(ipack.WriteAndAdvance(face.mIndices[0]));
    CHECK(ipack.WriteAndAdvance(face.mIndices[1]));
    CHECK(ipack.WriteAndAdvance(face.mIndices[2]));
  }

  CalcIndexedTriangleListTangentAndBinormal(vdata.get(), idata.get());

  MeshData data;
  data.vdata = vdata;
  data.idata = idata;
  data.vmax = vmax + Vector3(0.001f, 0.001f, 0.001f);
  data.vmin = vmin - Vector3(0.001f, 0.001f, 0.001f);
  return data;
}
}  // namespace
  
MeshLoadUtil::MeshLoadUtil(FileSystem* fs)
    : fsystem_(fs) {
}

const aiScene* MeshLoadUtil::LoadScene(const azer::ResPath& path, 
                                      Assimp::Importer* importer, 
                                      uint32_t flags) {
  std::vector<uint8_t> contents;
  FilePtr file = fsystem_->OpenFile(path);
  if (!file.get()) {
    LOG(ERROR) << "Failed to open file: " << path.fullpath();
    return NULL;
  }

  if (!file->PRead(0, -1, &contents)) {
    LOG(ERROR) << "Failed to read file: " << path.fullpath();
    return NULL;
  }

  const aiScene* scene = importer->ReadFileFromMemory(
      (const void*)&contents[0], contents.size(), flags, 
      ::base::UTF16ToUTF8(path.BaseName().as_string()).c_str());
  return scene;
}

EntityVecPtr MeshLoadUtil::LoadVertexData(const ResPath& path, VertexDesc* desc) {
  uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals 
      | aiProcess_FlipUVs;
  Assimp::Importer importer;
  const aiScene* scene = LoadScene(path, &importer, flags);
  if (scene == NULL) {
    LOG(ERROR) << "Failed to load file: " << path.fullpath();
    return EntityVecPtr();
  } 

  EntityVecPtr vecptr(new EntityVec);
  std::vector<int32_t> mtrl_index;
  Vector3 vmin  = Vector3(999999.9f, 999999.9f, 999999.9f);
  Vector3 vmax = Vector3(-999999.9f, -999999.9f, -999999.9f);
  for (uint32_t i = 0; i < scene->mNumMeshes; ++i) {
    MeshData data = LoadMeshData(scene->mMeshes[i], desc);
    VertexBufferGroupPtr vbg = new VertexBufferGroup(data.vdata.get());
    IndicesBufferPtr ib(new IndicesBuffer(data.idata.get()));
    EntityPtr entity(new Entity(vbg.get(), ib.get()));
    entity->set_vmin(data.vmin);
    entity->set_vmax(data.vmax);
    vecptr->AddEntity(entity.get());
  }

  return vecptr;
}

MeshPtr MeshLoadUtil::Load(const ResPath& path, VertexDesc* desc) {
  uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals 
      | aiProcess_FlipUVs;
  Assimp::Importer importer;
  const aiScene* scene = LoadScene(path, &importer, flags);
  if (scene == NULL) {
    LOG(ERROR) << "Failed to load file: " << path.fullpath();
    return NULL;
  } 

  MeshPtr mesh(new Mesh);
  std::vector<int32_t> mtrl_index;
  Vector3 vmin  = Vector3(999999.9f, 999999.9f, 999999.9f);
  Vector3 vmax = Vector3(-999999.9f, -999999.9f, -999999.9f);
  for (uint32_t i = 0; i < scene->mNumMeshes; ++i) {
    MeshPartPtr part = new MeshPart(NULL);
    const aiMesh* paiMesh = scene->mMeshes[i];
    MeshData data = LoadMeshData(scene->mMeshes[i], desc);
    VertexBufferGroupPtr vbg = new VertexBufferGroup(data.vdata.get());
    IndicesBufferPtr ib(new IndicesBuffer(data.idata.get()));
    EntityPtr entity(new Entity(vbg.get(), ib.get()));
    entity->set_vmin(data.vmin);
    entity->set_vmax(data.vmax);
    mtrl_index.push_back(paiMesh->mMaterialIndex);
    part->AddEntity(entity.get());
    mesh->AddMeshPart(part.get());
  }

  return mesh;
}
}  // namespace azer
