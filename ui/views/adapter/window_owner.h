#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
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
 protected:
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

  void OnKeyEvent(ui::KeyEvent* event) override;
  void OnMouseEvent(ui::MouseEvent* event) override;
  void OnScrollEvent(ui::ScrollEvent* event) override;
  void OnTouchEvent(ui::TouchEvent* event) override;
  void OnGestureEvent(ui::GestureEvent* event) override;

 private:
  View* view_;
  DISALLOW_COPY_AND_ASSIGN(ViewBridge);
};

class VIEWS_EXPORT WindowOwner {
 public:
  WindowOwner();
  ~WindowOwner();

  ui::Layer* layer();
  const ui::Layer* layer() const;

  aura::Window* Create(View* view);
  void Destroy();
  void Attach(aura::Window* window);
  void Detach();

  aura::Window* window() { return window_;}
  const aura::Window* window() const { return window_;}

  void OnAddChildViewAt(View* view, int index);
  void OnRemoveChildView(View* view);
 private:
  View* view_;
  scoped_ptr<ViewBridge> bridge_;
  aura::Window* window_;
  bool attached_;
  DISALLOW_COPY_AND_ASSIGN(WindowOwner);
};

}  // namespace views
