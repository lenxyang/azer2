#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/i18n/rtl.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/cursor/cursor.h"
#include "ui/events/event.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/drop_target_event.h"
#include "ui/base/dragdrop/os_exchange_data.h"

#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/views/views_export.h"

namespace ui {
class NativeTheme;
class ThemeProvider;
}

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
  int32 child_count() const { return static_cast<int32>(children_.size());}
  View* child_at(int32 index) { return children_.at(index);}

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
  virtual bool HasFocus() const;

  // Request keyboard focus. The receiving view will become the focused view.
  virtual void RequestFocus();

  void SetEnabled(bool enabled);
  bool enabled() const { return enabled_;}


  // Get the theme provider from the parent widget.
  ui::ThemeProvider* GetThemeProvider() const;

  // Returns the NativeTheme to use for this View. This calls through to
  // GetNativeTheme() on the Widget this View is in. If this View is not in a
  // Widget this returns ui::NativeTheme::instance().
  ui::NativeTheme* GetNativeTheme() {
    return const_cast<ui::NativeTheme*>(
        const_cast<const View*>(this)->GetNativeTheme());
  }
  const ui::NativeTheme* GetNativeTheme() const;


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
  virtual const char* GetClassName() const;

  void InvalidateLayout();

  virtual gfx::Size GetPreferredSize() const;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds);
  
  void OnPaint(gfx::Canvas* canvas) override;
  virtual void OnPaintBackground(gfx::Canvas* canvas);
  virtual void OnPaintBorder(gfx::Canvas* canvas);
  virtual void ChildPreferredSizeChanged(View* child) {}
  virtual void PreferredSizeChanged();

  // The background object is owned by this object and may be NULL.
  void set_background(Background* b);
  const Background* background() const { return background_.get(); }
  Background* background() { return background_.get(); }

  // The border object is owned by this object and may be NULL.
  virtual void SetBorder(scoped_ptr<Border> b);
  const Border* border() const { return border_.get(); }
  Border* border() { return border_.get(); }

  virtual void OnFocus();
  virtual void OnBlur();
  virtual void VisibilityChanged(View* starting_from, bool is_visible);
  virtual void OnEnabledChanged();

  // Returns whether we're in the middle of a drag session that was initiated
  // by us.
  bool InDrag();
  virtual bool CanDrop(const ui::OSExchangeData& data);
  virtual void OnDragEntered(const ui::DropTargetEvent& event);
  virtual int OnDragUpdated(const ui::DropTargetEvent& event);
  virtual void OnDragExited();
  virtual int OnPerformDrop(const ui::DropTargetEvent& event);
  virtual void OnDragDone();

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


  // Returns true if the mouse cursor is over |view| and mouse events are
  // enabled.
  bool IsMouseHovered();

  // This method is invoked when the user clicks on this view.
  // The provided event is in the receiver's coordinate system.
  //
  // Return true if you processed the event and want to receive subsequent
  // MouseDraggged and MouseReleased events.  This also stops the event from
  // bubbling.  If you return false, the event will bubble through parent
  // views.
  //
  // If you remove yourself from the tree while processing this, event bubbling
  // stops as if you returned true, but you will not receive future events.
  // The return value is ignored in this case.
  //
  // Default implementation returns true if a ContextMenuController has been
  // set, false otherwise. Override as needed.
  //
  virtual bool OnMousePressed(const ui::MouseEvent& event);

  // This method is invoked when the user clicked on this control.
  // and is still moving the mouse with a button pressed.
  // The provided event is in the receiver's coordinate system.
  //
  // Return true if you processed the event and want to receive
  // subsequent MouseDragged and MouseReleased events.
  //
  // Default implementation returns true if a ContextMenuController has been
  // set, false otherwise. Override as needed.
  //
  virtual bool OnMouseDragged(const ui::MouseEvent& event);

  // This method is invoked when the user releases the mouse
  // button. The event is in the receiver's coordinate system.
  //
  // Default implementation notifies the ContextMenuController is appropriate.
  // Subclasses that wish to honor the ContextMenuController should invoke
  // super.
  virtual void OnMouseReleased(const ui::MouseEvent& event);

  // This method is invoked when the mouse press/drag was canceled by a
  // system/user gesture.
  virtual void OnMouseCaptureLost();

  // This method is invoked when the mouse is above this control
  // The event is in the receiver's coordinate system.
  //
  // Default implementation does nothing. Override as needed.
  virtual void OnMouseMoved(const ui::MouseEvent& event);

  // This method is invoked when the mouse enters this control.
  //
  // Default implementation does nothing. Override as needed.
  virtual void OnMouseEntered(const ui::MouseEvent& event);

  // This method is invoked when the mouse exits this control
  // The provided event location is always (0, 0)
  // Default implementation does nothing. Override as needed.
  virtual void OnMouseExited(const ui::MouseEvent& event);

  // Invoked when a key is pressed or released.
  // Subclasser should return true if the event has been processed and false
  // otherwise. If the event has not been processed, the parent will be given a
  // chance.
  virtual bool OnKeyPressed(const ui::KeyEvent& event);
  virtual bool OnKeyReleased(const ui::KeyEvent& event);

  // Invoked when the user uses the mousewheel. Implementors should return true
  // if the event has been processed and false otherwise. This message is sent
  // if the view is focused. If the event has not been processed, the parent
  // will be given a chance.
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event);
 protected:
  bool ProcessMousePressed(const ui::MouseEvent& event);
  bool ProcessMouseDragged(const ui::MouseEvent& event);
  void ProcessMouseReleased(const ui::MouseEvent& event);

  // Invoked when the NativeTheme associated with this View changes.
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) {}

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

  void PropagateNativeThemeChanged(const ui::NativeTheme* theme);

  virtual void InitAuraWindow();

  scoped_ptr<aura::Window> window_;
  Views children_;
  View* parent_;
  RootView* root_;

  gfx::Rect bounds_;
  bool focusable_;
  bool visible_;
  bool enabled_;

  scoped_ptr<Background> background_;
  scoped_ptr<Border> border_;
  DISALLOW_COPY_AND_ASSIGN(View);
};
}  // namespace views
