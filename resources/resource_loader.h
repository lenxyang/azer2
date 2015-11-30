#pragma once

#include "base/memory/ref_counted.h"

#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/base/res_path.h"
#include "azer/base/file_system.h"

namespace azer {
class ResourceManager;
class RepositoryNode;
typedef scoped_refptr<RepositoryNode> RepositoryNodePtr;

class AZER_EXPORT ResourceLoader {
 public:
  ResourceLoader();
  virtual ~ResourceLoader();

  // load resource into RepositoryNode   
  virtual ResourcePtr LoadResource(const ResPath& path,
                                   const std::vector<uint8>& contents,
                                   ResourceManager* manager) = 0;
 protected:
  DISALLOW_COPY_AND_ASSIGN(ResourceLoader);
};

}  // namespace azer
