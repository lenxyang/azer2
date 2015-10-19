#include "azer/render/viewport.h"

namespace azer {
Viewport::Viewport(int32 l, int32 t, int32 w, int32 h, float mindepth,
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
