#include "azer/ui/views/widget/window_owner.h"

#include "base/logging.h"
#include "azer/ui/aura/window.h"

namespace views {
WindowOwner::WindowOwner()
    : window_(NULL) {
}

WindowOwner::~WindowOwner() {
}

ui::Layer* WindowOwner::layer() {
  CHECK(!window_);
  return window_->layer();
}

aura::Window* WindowOwner::Create(aura::WindowDelegate* delegate) {
  CHECK(!window_);
  window_ = new aura::Window(delegate);
  return window_;
}

void WindowOwner::SetWindow(aura::Window* window) {
  CHECK(!window_);
}
}  // namespace views
