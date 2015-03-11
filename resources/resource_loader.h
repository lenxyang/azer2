#pragma once

#include "base/memory/ref_counted.h"

#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/resources/file_path.h"
#include "azer/resources/file_system.h"

namespace azer {
namespace resources {

class RepositoryNode;
typedef scoped_refptr<RepositoryNode> RepositoryNodePtr;

class AZER_EXPORT ResourceLoader {
 public:
  ResourceLoader(Resource::Type type) : type_(type){}
  virtual ~ResourceLoader() {}

  // load resource into RepositoryNode   
  virtual bool LoadResource(const ResFilePath& path, RepositoryNodePtr parent,
                            const FileContent* content) = 0;

  Resource::Type type() const { return type_; }
 protected:
  const Resource::Type type_;
  DISALLOW_COPY_AND_ASSIGN(ResourceLoader);
};

// get the resource loader
class AZER_EXPORT ResourceLoaderManager {
 public:
  ResourceLoaderManager() {}
  virtual ~ResourceLoaderManager() {}
  virtual ResourceLoader* GetResourceLoader(const ResFilePath& path,
                                            const FileContent* content) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(ResourceLoaderManager);
};
}  // namespace resource
}  // namespace azer
