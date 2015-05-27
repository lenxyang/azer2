#pragma once

#include <map>
#include "base/basictypes.h"

#include "azer/base/resource.h"
#include "azer/files/res_path.h"

namespace azer {
class AZER_EXPORT ResourceCache {
 public:
  ResourceCache();
  ~ResourceCache();

  typedef std::map<ResPath, ResourcePtr> CacheDict;
 private:
  CacheDict cache_;
  DISALLOW_COPY_AND_ASSIGN(ResourceCache);
};
}  // namespace azer
