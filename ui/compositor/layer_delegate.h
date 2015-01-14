#pragma once

#include "azer/base/export.h"

namespace gfx {
class Canvas;
class Rect;
}  // namespace ui

namespace azer {
namespace compositor {
class AZER_EXPORT LayerDelegate {
 public:
  virtual void OnPaintLayer(gfx::Canvas* canvas) = 0;
 protected:
  virtual ~LayerDelegate() {}
};
}  // namespace compositor
}  // namespace azer
