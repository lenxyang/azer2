#pragma once

#include "azer/files/res_path.h"
#include "azer/base/resource.h"
#include "azer/resources/repository_node.h"

namespace azer {

namespace resources {
class ResourceLoader;
class ResourceLoaderManager;
}  // namespace resources

class ResourceContext;
class SceneNode;

class AZER_EXPORT ResourceManager {
 public:
  ResourceManager(ResourceContext* context);
  virtual ~ResourceManager();

  ResourcePtr GetResource(const ResPath& path); 

  // release all resource binding with scene node
  void ReleaseSceneResource(SceneNode* node);
 private:
  // load resource into repositor node
  bool LoadResourceSync(const ResPath& path, ResourcePtr* ptr); 
  ResourceContext* context_;
  resources::RepositoryNodePtr root_;
  DISALLOW_COPY_AND_ASSIGN(ResourceManager);
};
}  // namespace azer
