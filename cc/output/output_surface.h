#pragma once

#include "azer/base/export.h"
#include "cc/output/output_surface.h"

namespace azer {
namespace cc {
class AZER_EXPORT AzerOutputSurface : public ::cc::OutputSurface {
 public:
 protected:
  DISALLOW_COPY_AND_ASSIGN(AzerOutputSurface);
};
}  // namespace cc
}  // namespace azer
