#include "azer/resources/resource_manager.h"
#include "azer/resources/context.h"
#include "azer/resources/resource_loader.h"

namespace azer {
ResourceManager::ResourceManager(ResourceContext* context)
    : context_(context) {
}

ResourceManager::~ResourceManager() {
}

ResourcePtr ResourceManager::GetResource(const ResPath& path) {
  DCHECK(!path.component().empty());
  ResourcePtr resptr = root_->GetResource(path);
  if (resptr.get()) { return resptr;}

  if (LoadResourceSync(path, &respth)) {
    GenerateTreeHierarchy(path, root);
    RepositoryNodePtr node = root_->GetNode(path);
    node->AddResource(path.component());
    return node;
  } else {
    return ResourcePtr();
  }
}

bool ResourceManager::LoadResourceSync(const ResPath& path, ResourcePtr* ptr) {
  FileSystem* fs = context_->GetFileSystem();
  FileContentPtr content = fs->LoadFile(path);
  if (content.get()) {
    ResourceLoaderManager* mgr = context_->GetResourceLoaderManager();
    ResourceLoader* loader = mgr->GetResourceLoader(path, content.get());
    *ptr = loader->LoadResource(path, content.get());
    return ptr->get() != NULL;
  } else {
    return false;
  }
}

void ResourceManager::ReleaseSceneResource(SceneNode* node) {
}
}  // namespace azer
