#pragma once

#include "azer/azer.h"
#include "azer/base/file_system.h"

struct aiScene;
struct aiMesh;
namespace Assimp {
class Importer;
}

namespace azer {
class MeshLoadUtil {
 public:
  MeshLoadUtil(FileSystem* fs);

  MeshPtr Load(const ResPath& path, VertexDesc* desc);
  EntityVecPtr LoadVertexData(const ResPath& path, VertexDesc* desc);
 private:
  const aiScene* LoadScene(const ResPath& path, Assimp::Importer* importer, 
                           uint32 flags);
  FileSystem* fsystem_;
  DISALLOW_COPY_AND_ASSIGN(MeshLoadUtil);
};
}  // namespace azer


