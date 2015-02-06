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

gfx::Size ViewBridge::GetMinimumSize() const {
  return gfx::Size(1, 1);
}

gfx::Size ViewBridge::GetMaximumSize() const {
  return gfx::Size(800, 600);
}

void ViewBridge::OnBoundsChanged(const gfx::Rect& old_bounds,
                             const gfx::Rect& new_bounds)  {
}

gfx::NativeCursor ViewBridge::GetCursor(const gfx::Point& point) {
  return gfx::kNullCursor;
}

int ViewBridge::GetNonClientComponent(const gfx::Point& point) const {
  return 0;
}

bool ViewBridge::ShouldDescendIntoChildForEventHandling(aura::Window* child,
                                                    const gfx::Point& location) {
  return true;
}

bool ViewBridge::CanFocus() {
  return false;
}

void ViewBridge::OnCaptureLost() {
}

void ViewBridge::OnPaint(gfx::Canvas* canvas) {
}

void ViewBridge::OnDeviceScaleFactorChanged(float device_scale_factor) {
}

void ViewBridge::OnWindowDestroying(aura::Window* window) {
}

void ViewBridge::OnWindowDestroyed(aura::Window* window) {
}

void ViewBridge::OnWindowTargetVisibilityChanged(bool visible) {
}

bool ViewBridge::HasHitTestMask() const {
  return false;
}

void ViewBridge::GetHitTestMask(gfx::Path* mask) const {
}
}  // namespace views
