#pragma once

#include "azer/base/export.h"

namespace gfx {
class Canvas;
}

namespace azer {
namespace widget {

class AZER_EXPORT WidgetDelegate {
 public:
  virtual void OnPaint(gfx::Canvas* canvas) = 0;
};

}  // namespace widget
}  // namespace azer


