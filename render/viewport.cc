#include "azer/render/viewport.h"

namespace azer {
Viewport::Viewport(int32_t l, int32_t t, int32_t w, int32_t h, float mindepth,
                   float maxdepth)
    : bounds(l, t, w, h)
    , min_depth(mindepth), max_depth(maxdepth) {
}

Viewport::Viewport(const gfx::Rect& rect, float mindepth, float maxdepth)
    : bounds(rect)
    , min_depth(mindepth), max_depth(maxdepth) {
}

Viewport::Viewport()
    : bounds(0, 0, 800, 600)
    , min_depth(0.0f), max_depth(1.0f) {
}
}
