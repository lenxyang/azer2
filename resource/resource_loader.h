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
class EffectLib;
class ResourceLoader;
class ResourceSpecialLoader;
typedef scoped_refptr<ResourceSpecialLoader> ResourceSpecialLoaderPtr;

struct AZER_EXPORT ResourceLoadContext {
  ResourceLoader* loader;
  ResPath path;
  RepositoryNodePtr root;
  FileSystem* file_system;
  EffectLib* effectlib;
  EffectAdapterContext* effect_adapter_context;

  ResourceLoadContext();
};

class AZER_EXPORT ResourceSpecialLoader
    : public ::base::RefCounted<ResourceSpecialLoader> {
 public:
  virtual const char* GetLoaderName() const = 0;
  virtual bool CouldLoad(ConfigNode* node) const = 0;
  virtual VariantResource Load(const ConfigNode* node,
                               ResourceLoadContext* ctx) = 0;
};

class AZER_EXPORT ResourceLoader {
 public:
  explicit ResourceLoader(ResourceLoadContext* ctx);
  ~ResourceLoader();

  void RegisterSpecialLoader(ResourceSpecialLoader* loader);
  ResourceSpecialLoader* GetLoader(ConfigNode *node);

  FileSystem* file_system();
  VariantResource Load(const ResPath& path);
 private:
  std::map<std::string, ResourceSpecialLoaderPtr> dict_;
  ResourceLoadContext context_;
  DISALLOW_COPY_AND_ASSIGN(ResourceLoader);
};
}  // namespace azer
