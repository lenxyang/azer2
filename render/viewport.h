#pragma once

#include "ui/gfx/geometry/rect.h"
#include "azer/base/export.h"

namespace azer {
struct AZER_EXPORT Viewport {
  gfx::Rect bounds;
  int32 min_depth;
  int32 max_depth;

  Viewport(int32 l, int32 t, int32 w, int32 h, float mindepth = 0.0f,
           float maxdepth = 1.0f);
  Viewport(const gfx::Rect& rect, float mindepth = 0.0f, float maxdepth = 1.0f);
  Viewport();
};

}  // namespace 
