#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/views/views_export.h"

namespace aura {
class Window;
}  // namespace aura

namespace views {

class Background;
class Border;
class RootView;

class VIEWS_EXPORT View : public aura::WindowDelegate {
 public:
  typedef std::vector<View*> Views;
  View();
  virtual ~View();

  void AddChildView(View* view);
  void RemoveChildView(View* view);
  bool Contains(View* view);

  int id() const;
  void set_id(int id);

  const std::string& name() const;
  void SetName(const std::string& name);

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

  void SizeToPreferredSize();

  // Returns the bounds of the content area of the view, i.e. the rectangle
  // enclosed by the view's border.
  gfx::Rect GetContentsBounds() const;

  // Returns the bounds of the view in its own coordinates (i.e. position is
  // 0, 0).
  gfx::Rect GetLocalBounds() const;

  void SetFocusable(bool focusable) { focusable_ = focusable;}
  bool focusable() const { return focusable_;}
  virtual bool HasFocus();

  // Returns the insets of the current border. If there is no border an empty
  // insets is returned.
  virtual gfx::Insets GetInsets() const;

  // Returns the visible bounds of the receiver in the receivers coordinate
  // system.
  //
  // When traversing the View hierarchy in order to compute the bounds, the
  // function takes into account the mirroring setting and transformation for
  // each View and therefore it will return the mirrored and transformed version
  // of the visible bounds if need be.
  gfx::Rect GetVisibleBounds() const;

  // Return the bounds of the View in screen coordinate system.
  gfx::Rect GetBoundsInScreen() const;

  // Returns the baseline of this view, or -1 if this view has no baseline. The
  // return value is relative to the preferred height.
  virtual int GetBaseline() const;

  // Return the height necessary to display this view with the provided width.
  // View's implementation returns the value from getPreferredSize.cy.
  // Override if your View's preferred height depends upon the width (such
  // as with Labels).
  virtual int GetHeightForWidth(int w) const;

  virtual void Layout();

  virtual void Show();
  virtual void Hide();

  bool visible() const { return visible_;}
  

  aura::Window* window() { return window_.get();}
  const aura::Window* window() const { return window_.get();}

  RootView* GetRootView() { return root_;}
  const RootView* GetRootView() const { return root_;}
  virtual const char* GetClassName();

  virtual gfx::Size GetPreferredSize() const;
  void OnPaint(gfx::Canvas* canvas) override;
  virtual void OnPaintBackground(gfx::Canvas* canvas);
  virtual void OnPaintBorder(gfx::Canvas* canvas);

  // Return the cursor that should be used for this view or the default cursor.
  // The event location is in the receiver's coordinate system. The caller is
  // responsible for managing the lifetime of the returned object, though that
  // lifetime may vary from platform to platform. On Windows and Aura,
  // the cursor is a shared resource.
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event);

  // A convenience function which calls HitTestRect() with a rect of size
  // 1x1 and an origin of |point|. |point| is in the local coordinate space
  // of |this|.
  bool HitTestPoint(const gfx::Point& point) const;

  // Returns true if |rect| intersects this view's bounds. |rect| is in the
  // local coordinate space of |this|.
  bool HitTestRect(const gfx::Rect& rect) const;

  void SchedulePaint();
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
  // void OnPaint(gfx::Canvas* canvas) override;
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

  // when view attached to parent with root.
  void OnAttachedRecusive(RootView* view);
  void OnDetachRecusive();

  scoped_ptr<aura::Window> window_;
  Views children_;
  View* parent_;
  RootView* root_;

  gfx::Rect bounds_;
  bool focusable_;
  bool visible_;

  scoped_ptr<Background> background_;
  scoped_ptr<Border> border_;
  DISALLOW_COPY_AND_ASSIGN(View);
};
}  // namespace views
