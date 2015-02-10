#pragma once

#include "base/memory/ref_counted.h"

#include "azer/base/export.h"
#include "azer/base/resource.h"

namespace azer {
class AZER_EXPORT ResourceLoader {
 public:
  ResourceLoader() {}
  virtual ~ResourceLoader() {}
  virtual ResourcePtr LoadResource(ResourceData* data) = 0;
 protected:
  Resource::Type type_;
  DISALLOW_COPY_AND_ASSIGN(ResourceLoader);
};
}  // namespace azer
