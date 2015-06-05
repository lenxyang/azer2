#pragma once

#include "azer/base/export.h"
#include "azer/files/res_path.h"
#include "azer/files/file_content.h"

namespace azer {
class ResourceLoader;
class AZER_EXPORT ResourceContext {
 public:
  virtual FileContentPtr LoadFile(const ResPath& path) = 0;
  virtual ResourceLoader* GetResourceLoader(const ResPath& path,
                                            const FileContent* content) = 0;
};
}  // namespace azer
