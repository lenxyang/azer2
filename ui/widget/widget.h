#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/cursor/cursor.h"

#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/widget/widgets_export.h"

namespace aura {
class Window;
}  // namespace aura

namespace widget {
class WIDGET_EXPORT Widget : public aura::WindowDelegate {
 public:
  typedef std::vector<Widget*> Widgets;
  Widget();
  virtual ~Widget();

  void AddChild(Widget* widget);
  void RemoveChild(Widget* widget);
  bool Contains(Widget* widget);

  int id() const;
  void set_id(int id);

  const std::string& name() const;
  void SetName(const std::string& name);

  void Show();
  void Hide();

  aura::Window* window() { return window_.get();}
  const aura::Window* window() const { return window_.get();}

  Widget* GetRootWidget() { return root_;}
  const Widget* GetRootWidget() const { return root_;}
  virtual const char* GetClassName();
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

  scoped_ptr<aura::Window> window_;
  Widgets children_;
  Widget* parent_;
  Widget* root_;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};
}  // namespace widget
