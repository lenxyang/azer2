#pragma once

#include <memory>

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/common.h"

namespace azer {
class AZER_EXPORT GpuResourceView : public ::base::RefCounted<GpuResourceView> {
 public:
 private:
  DISALLOW_COPY_AND_ASSIGN(GpuResourceView);
};
}  // namespace azer
