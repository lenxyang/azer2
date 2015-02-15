#pragma once

#include "azer/ui/skinned/skinned_export.h"

namespace skinned {
class SKIN_EXPORT SkinnedGroup {
 public:
  SkinnedGroup();
  virtual ~SkinnedGroup();
 private:
  DISALLOW_COPY_AND_ASSIGN(SkinnedGroup);
};
}  // namespace skinned
