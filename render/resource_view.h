#pragma once

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT ResourceView : public ::base::RefCounted<ResourceView> {
 public:
  ResourceView() {}
 private:
  DISALLOW_COPY_AND_ASSIGN(ResourceView);
};
}  // namespace azer
