#pragma once

#include "ui/gfx/geometry/rect.h"
#include "azer/base/export.h"

namespace azer {
struct AZER_EXPORT Viewport {
  gfx::Rect bounds;
  int32_t min_depth;
  int32_t max_depth;

  Viewport(int32_t l, int32_t t, int32_t w, int32_t h, float mindepth = 0.0f,
           float maxdepth = 1.0f);
  Viewport(const gfx::Rect& rect, float mindepth = 0.0f, float maxdepth = 1.0f);
  Viewport();
};

}  // namespace 
