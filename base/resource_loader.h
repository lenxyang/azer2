#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "azer/base/repository_node.h"

namespace azer {
class ConfigNode;
class EffectAdapterContext;
class EffectLib;
class FileSystem;
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

AZER_EXPORT VariantResource LoadReferResource(const ConfigNode* node,
                                              ResourceLoadContext* ctx);
AZER_EXPORT VariantResource LoadResource(const ResPath& path, int type,
                                         ResourceLoadContext* ctx);
AZER_EXPORT VariantResource LoadResource(const ResPath& path, int type,
                                         ResourceLoader* loader);
AZER_EXPORT ConfigNode* GetTypedReferNode(const std::string& type_name,
                                          const ConfigNode* parent);
}  // namespace azer
