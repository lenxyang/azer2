#include "azer/ui/views/view.h"

namespace views {

gfx::Size View::GetMinimumSize() const {
  return gfx::Size(1, 1);
}

gfx::Size View::GetMaximumSize() const {
  return gfx::Size(800, 600);
}

void View::OnBoundsChanged(const gfx::Rect& old_bounds,
                             const gfx::Rect& new_bounds)  {
}

gfx::NativeCursor View::GetCursor(const gfx::Point& point) {
  return cursor_;
}

int View::GetNonClientComponent(const gfx::Point& point) const {
  return 0;
}

bool View::ShouldDescendIntoChildForEventHandling(aura::Window* child,
                                                    const gfx::Point& location) {
  return true;
}

bool View::CanFocus() {
  return false;
}

void View::OnCaptureLost() {
}

void View::OnPaint(gfx::Canvas* canvas) {
}

void View::OnDeviceScaleFactorChanged(float device_scale_factor) {
}

void View::OnWindowDestroying(aura::Window* window) {
}

void View::OnWindowDestroyed(aura::Window* window) {
}

void View::OnWindowTargetVisibilityChanged(bool visible) {
}

bool View::HasHitTestMask() const {
  return false;
}

void View::GetHitTestMask(gfx::Path* mask) const {
}
}  // namespace views
