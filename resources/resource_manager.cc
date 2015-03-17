#include "azer/resources/resource_manager.h"
#include "azer/resources/context.h"
#include "azer/resources/resource_loader.h"
#include "azer/resources/resource_manager.h"

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

  if (LoadResourceSync(path, &resptr)) {
    GenerateTreeHierarchy(path, root_);
    RepositoryNodePtr node = root_->GetNode(path);
    node->AddLocalResource(path.component(), resptr);
    return resptr;
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
    *ptr = loader->LoadResource(path, content.get(), this);
    return ptr->get() != NULL;
  } else {
    return false;
  }
}

void ResourceManager::ReleaseSceneResource(SceneNode* node) {
}
}  // namespace azer
