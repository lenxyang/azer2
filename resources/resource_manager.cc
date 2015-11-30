#include "azer/resources/resource_manager.h"

#include "base/logging.h"

#include "azer/base/file_system.h"
#include "azer/resources/resource_context.h"
#include "azer/resources/resource_loader.h"
#include "azer/resources/resource_manager.h"

namespace azer {

ResourceManager::ResourceManager(ResourceContext* context)
    : context_(context) {
  root_ = new RepositoryNode(AZER_LITERAL("//"));
}

ResourceManager::~ResourceManager() {
}

ResourcePtr ResourceManager::GetResource(const ResPath& path) {
  ResourcePtr resptr = root_->GetResource(path);
  if (resptr.get()) { return resptr;}

  if (LoadResourceSync(path, &resptr)) {
    GenerateTreeHierarchy(path, root_);
    RepositoryNodePtr node = root_->GetNode(path);
    node->AddLocalResource(path.component().as_string(), resptr);
    return resptr;
  } else {
    return ResourcePtr();
  }
}

void ResourceManager::RegisterResource(const ResPath& path, ResourcePtr& resource) {
  DCHECK(resource.get());
  root_->AddResource(path, resource);
}

bool ResourceManager::LoadResourceSync(const ResPath& path, ResourcePtr* ptr) {
  FileContents contents;
  if (!LoadFileContents(path, &contents, context_->GetFileSystem())) {
    return false;
  }
  ResourceLoader* loader = context_->GetResourceLoader(path, contents);
  if (loader) {
    *ptr = loader->LoadResource(path, contents, this);
    return ptr->get() != NULL;
  } else {
    return false;
  }
}

bool ResourceManager::ReleaseResource(const ResPath& path) {
  return true;
}
}  // namespace azer
