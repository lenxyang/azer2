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

  virtual void Show();
  virtual void Hide();

  aura::Window* window() { return window_.get();}
  const aura::Window* window() const { return window_.get();}

  RootView* GetRootView() { return root_;}
  const RootView* GetRootView() const { return root_;}
  virtual const char* GetClassName();

  virtual gfx::Size GetPreferredSize() const;
  void OnPaint(gfx::Canvas* canvas) override;
  virtual void OnPaintBackground(gfx::Canvas* canvas);
  virtual void OnPaintBorder(gfx::Canvas* canvas);
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


  //
  void OnAttachedRecusive(RootView* view);
  void OnDetachRecusive();

  scoped_ptr<aura::Window> window_;
  Views children_;
  View* parent_;
  RootView* root_;

  gfx::Rect bounds_;

  scoped_ptr<Background> background_;
  scoped_ptr<Border> border_;
  DISALLOW_COPY_AND_ASSIGN(View);
};
}  // namespace views
