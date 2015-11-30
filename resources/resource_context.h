#pragma once

#include "azer/base/export.h"
#include "azer/base/res_path.h"

namespace azer {
class FileSystem;
class ResourceLoader;

class AZER_EXPORT ResourceContext {
 public:
  ResourceContext() {}
  virtual ~ResourceContext() {}

  FileSystem* GetFileSystem() { return file_system_;}
  virtual ResourceLoader* GetResourceLoader(const ResPath& path,
                                            const std::vector<uint8>& content) = 0;
  virtual bool GetResourcePath(const ResPath& path, base::FilePath* file_path) = 0;
 protected:
  FileSystem* file_system_;
  DISALLOW_COPY_AND_ASSIGN(ResourceContext);
};
}  // namespace azer
