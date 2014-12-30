#pragma once

#include "azer/base/export.h"
#include "cc/output/direct_renderer.h"

namespace azer {
namespace cc {
class AZER_EXPORT AzerRenderer : public ::cc::DirectRenderer {
 public:
 private:
  DISALLOW_COPY_AND_ASSIGN(AzerRenderer);
};
}  // namespace cc
}  // namespace azer
