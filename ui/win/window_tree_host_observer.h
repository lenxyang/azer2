#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace gfx {
class Point;
}

namespace azer {
namespace ui {
class AZER_EXPORT WindowTreeHostObserver {
 public:
  WindowTreeHostObserver() {}

  virtual void OnHostResized(const WindowTreeHost* host) {}
  virtual void OnHostMoved(const WindowTreeHost* host,
                           const gfx::Point& new_origin) {}

  virtual void OnHostCloseRequested(const WindowTreeHost* host) {}
 protected:
  virtual ~WindowTreeHostObserver() {}
};
}  // namespace ui
}  // namespace azer
