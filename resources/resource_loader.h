#pragma once

#include "base/memory/ref_counted.h"

#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/resources/file_path.h"
#include "azer/resources/file_system.h"

namespace azer {
class AZER_EXPORT ResourceLoader {
 public:
  ResourceLoader() {}
  virtual ~ResourceLoader() {}
  virtual ResourcePtr LoadResource(const ResFilePath& path,
                                   const FileContent* content) = 0;
 protected:
  Resource::Type type_;
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
}  // namespace azer
