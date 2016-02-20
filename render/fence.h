#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT Fence {
 public:
  virtual ~Fence() {}

  virtual bool Begin(bool current) = 0;
  virtual bool End() = 0;
 protected:
  Fence() {}
  DISALLOW_COPY_AND_ASSIGN(Fence);
};
}  // namespace azer
