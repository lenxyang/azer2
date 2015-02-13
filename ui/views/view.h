#pragma once


#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/events/event.h"

#include "azer/ui/views/views_export.h"
#include "azer/ui/aura/window_delegate.h"

namespace aura {
class Window;
class WindowDelegate;
};

namespace views {
class View : public aura::WindowDelegate {
 public:
  explicit View(View* view);
  ~View() override;

  void SetBounds(int x, int y, int width, int height);
  void SetBoundsRect(const gfx::Rect& bounds);
  void SetSize(const gfx::Size& size);
  void SetPosition(const gfx::Point& position);
  void SetX(int x);
  void SetY(int y);

  // No transformation is applied on the size or the locations.
  const gfx::Rect& bounds() const { return bounds_; }
  int x() const { return bounds_.x(); }
  int y() const { return bounds_.y(); }
  int width() const { return bounds_.width(); }
  int height() const { return bounds_.height(); }
  const gfx::Size& size() const { return bounds_.size(); }

  gfx::Rect GetContentsBounds() const;
  gfx::Rect GetLocalBounds() const;
  gfx::Rect GetVisibleBounds() const;

  void SetEnabled(bool enabled);
  bool enabled() const { return enabled_; }

  // Get the size the View would like to be, if enough space were available.
  virtual gfx::Size GetPreferredSize() const;

  // Convenience method that sizes this view to its preferred size.
  void SizeToPreferredSize();

  // The view class name.
  static const char kViewClassName[];

  // Return the receiving view's class name. A view class is a string which
  // uniquely identifies the view class. It is intended to be used as a way to
  // find out during run time if a view can be safely casted to a specific view
  // subclass. The default implementation returns kViewClassName.
  virtual const char* GetClassName() const;

  int id() const;
  int group_id() const;

  // The background object is owned by this object and may be NULL.
  void set_background(Background* b);
  const Background* background() const { return background_.get(); }
  Background* background() { return background_.get(); }

  // The border object is owned by this object and may be NULL.
  virtual void SetBorder(scoped_ptr<Border> b);
  const Border* border() const { return border_.get(); }
  Border* border() { return border_.get(); }

  bool HitTestPoint(const gfx::Point& point) const;
  bool HitTestRect(const gfx::Rect& rect) const;
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
  scoped_ptr<aura::Window> window_;
  DISALLOW_COPY_AND_ASSIGN(View);
};

}  // namespace views
