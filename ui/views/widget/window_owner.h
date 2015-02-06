#pragma once

#include "base/basictypes.h"
#include "azer/ui/views/views_export.h"

namespace ui {
class Layer;
}

namespace aura {
class Window;
class WindowDelegate;
};

namespace views {

class VIEWS_EXPORT WindowOwner {
 public:
  WindowOwner();
  ~WindowOwner();

  ui::Layer* layer();

  aura::Window* Create(aura::WindowDelegate* delegate);
  void SetWindow(aura::Window* window);

  aura::Window* window() { return window_;}
  const aura::Window* window() const { return window_;}
 private:
  aura::Window* window_;
  DISALLOW_COPY_AND_ASSIGN(WindowOwner);
};

}  // namespace views
