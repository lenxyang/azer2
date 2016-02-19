#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "azer/base/repository_node.h"
#include "azer/base/config_node.h"
#include "azer/base/file_system.h"
#include "azer/resource/variant_resource.h"

namespace azer {
class ResourceLoader;
class ResourceSpecialLoader;
typedef scoped_refptr<ResourceSpecialLoader> ResourceSpecialLoaderPtr;

struct ResourceLoadContext {
  ResourceLoader* loader;
  ResPath path;
  RepositoryNodePtr root;
  FileSystem* filesystem;
};

class ResourceSpecialLoader : public ::base::RefCounted<ResourceSpecialLoader> {
 public:
  virtual const char* GetLoaderName() const = 0;
  virtual bool CouldLoad(ConfigNode* node) const = 0;
  virtual VariantResource Load(const ConfigNode* node,
                               ResourceLoadContext* ctx) = 0;
};

class ResourceLoader {
 public:
  explicit ResourceLoader(FileSystem* fs);
  ~ResourceLoader();

  void RegisterSpecialLoader(ResourceSpecialLoader* loader);
  ResourceSpecialLoader* GetLoader(ConfigNode *node);

  FileSystem* file_system() { return filesystem_;}
  VariantResource Load(const ResPath& path);
 private:
  std::map<std::string, ResourceSpecialLoaderPtr> dict_;
  FileSystem* filesystem_;
  DISALLOW_COPY_AND_ASSIGN(ResourceLoader);
};
}  // namespace azer
