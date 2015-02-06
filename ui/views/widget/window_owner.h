#pragma once

#include "base/basictypes.h"
#include "ui/base/cursor/cursor.h"

#include "azer/ui/views/views_export.h"
#include "azer/ui/aura/window_delegate.h"

namespace ui {
class Layer;
}

namespace aura {
class Window;
class WindowDelegate;
};

namespace views {

class View;

class ViewBridge : public aura::WindowDelegate {
 public:
  explicit ViewBridge(View* view);
  ~ViewBridge() override;

  // Overridden from aura::WindowDelegate:
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  void OnBoundsChanged(const gfx::Rect& old_bounds,
                       const gfx::Rect& new_bounds) override;
  gfx::NativeCursor GetCursor(const gfx::Point& point) override;
  int GetNonClientComponent(const gfx::Point& point) const override;
  bool ShouldDescendIntoChildForEventHandling(
      aura::Window* child,
      const gfx::Point& location) override;
  bool CanFocus() override;
  void OnCaptureLost() override;
  void OnPaint(gfx::Canvas* canvas) override;
  void OnDeviceScaleFactorChanged(float device_scale_factor) override;
  void OnWindowDestroying(aura::Window* window) override;
  void OnWindowDestroyed(aura::Window* window) override;
  void OnWindowTargetVisibilityChanged(bool visible) override;
  bool HasHitTestMask() const override;
  void GetHitTestMask(gfx::Path* mask) const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(ViewBridge);
};

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
