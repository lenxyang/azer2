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
  return ResourcePtr();
}

bool ResourceManager::LoadResource(const ResPath& path, ResourcePtr* ptr) {
}

void ResourceManager::GenerateTreeHierarchy(const ResPath& path) {
}
}  // namespace azer
