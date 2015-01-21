#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "build/build_config.h"
#include "ui/accessibility/ax_enums.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/drop_target_event.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/ui_base_types.h"
#include "ui/events/event.h"
#include "ui/events/event_target.h"
#include "ui/gfx/geometry/r_tree.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d.h"

#include "azer/base/export.h"
#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/views/cull_set.h"

namespace gfx {
class Canvas;
class Insets;
class Path;
class Transform;
}


namespace azer {

namespace compositor {
class Layer;
}

namespace widget {
class Widget;
}  // namespace widget

namespace views {

class Border;
class Background;
class ViewEventObserver;

class AZER_EXPORT View : public widget::WidgetDelegate {
 public:
  typedef std::vector<View*> Views;
  explicit View(View* parent);
  ~View() override;

  widget::Widget* GetWidget() { return widget_.get();}
  const widget::Widget* GetWidget() const { return widget_.get();}

  void AddEventObserver(ViewEventObserver* observer);
  void RemoveEventObserver(ViewEventObserver* observer);
  
  virtual void Show();
  virtual void Hide();
  bool visible() const { return visible_;}

  void SetName(const std::string& name);
  const std::string& name() const { return name_;}

  // Size and disposition ------------------------------------------------------
  // Methods for obtaining and modifying the position and size of the view.
  // Position is in the coordinate system of the view's parent.
  // Position is NOT flipped for RTL. See "RTL positioning" for RTL-sensitive
  // position accessors.
  // Transformations are not applied on the size/position. For example, if
  // bounds is (0, 0, 100, 100) and it is scaled by 0.5 along the X axis, the
  // width will still be 100 (although when painted, it will be 50x50, painted
  // at location (0, 0)).

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

  // Returns the bounds of the content area of the view, i.e. the rectangle
  // enclosed by the view's border.
  gfx::Rect GetContentsBounds() const;

  // Returns the bounds of the view in its own coordinates (i.e. position is
  // 0, 0).
  gfx::Rect GetLocalBounds() const;

  // Returns the bounds of the layer in its own pixel coordinates.
  gfx::Rect GetLayerBoundsInPixel() const;

  // Returns the insets of the current border. If there is no border an empty
  // insets is returned.
  virtual gfx::Insets GetInsets() const;

  // Set whether this view is enabled. A disabled view does not receive keyboard
  // or mouse inputs. If |enabled| differs from the current value, SchedulePaint
  // is invoked. Also, clears focus if the focused view is disabled.
  void SetEnabled(bool enabled);

  // Returns whether the view is enabled.
  bool enabled() const { return enabled_; }

  // The background object is owned by this object and may be NULL.
  void set_background(Background* b);
  const Background* background() const { return background_.get(); }
  Background* background() { return background_.get(); }

  // The border object is owned by this object and may be NULL.
  virtual void SetBorder(scoped_ptr<Border> b);
  const Border* border() const { return border_.get(); }
  Border* border() { return border_.get(); }

  // focus
  virtual bool HasFocus() const;
  void SetFocusable(bool focusable);
  bool IsFocusable() const;

  // Request keyboard focus. The receiving view will become the focused view.
  virtual void RequestFocus();

  // A convenience function which calls HitTestRect() with a rect of size
  // 1x1 and an origin of |point|. |point| is in the local coordinate space
  // of |this|.
  bool HitTestPoint(const gfx::Point& point) const;

  // Returns true if |rect| intersects this view's bounds. |rect| is in the
  // local coordinate space of |this|.
  bool HitTestRect(const gfx::Rect& rect) const;

  // Called by the framework to paint a View. Performs translation and clipping
  // for View coordinates and language direction as required, allows the View
  // to paint itself via the various OnPaint*() event handlers and then paints
  // the hierarchy beneath it.
  virtual void Paint(gfx::Canvas* canvas, const CullSet& cull_set);

  static const char kViewClassName[];
  virtual const char* GetClassName() const;

  // Tree operations -----------------------------------------------------------
  int child_count() const { return static_cast<int>(children_.size()); }
  bool has_children() const { return !children_.empty(); }

  // Returns the child view at |index|.
  const View* child_at(int index) const {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, child_count());
    return children_[index];
  }
  View* child_at(int index) {
    return const_cast<View*>(const_cast<const View*>(this)->child_at(index));
  }

  // Returns the parent view.
  const View* parent() const { return parent_; }
  View* parent() { return parent_; }


  // Returns true if |view| is contained within this View's hierarchy, even as
  // an indirect descendant. Will return true if child is also this view.
  bool Contains(const View* view) const;

  // Mark all or part of the View's bounds as dirty (needing repaint).
  // |r| is in the View's coordinates.
  // Rectangle |r| should be in the view's coordinate system. The
  // transformations are applied to it to convert it into the parent coordinate
  // system before propagating SchedulePaint up the view hierarchy.
  // TODO(beng): Make protected.
  virtual void SchedulePaint();
  virtual void SchedulePaintInRect(const gfx::Rect& r);
 protected:
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
  View();  // used by RootView

  void InitWidget();
  compositor::Layer* layer();
  const compositor::Layer* layer() const;

  // Adds |view| as a child of this view, optionally at |index|.
  void AddChildView(View* view);
  void AddChildViewAt(View* view, int index);
  // Moves |view| to the specified |index|. A negative value for |index| moves
  // the view at the end.
  void ReorderChildView(View* view, int index);

  // Removes |view| from this view. The view's parent will change to NULL.
  void RemoveChildView(View* view);

  // Removes all the children from this view. If |delete_children| is true,
  // the views are deleted, unless marked as not parent owned.
  void RemoveAllChildViews(bool delete_children);

  // Overriden from widget::WidgetDelegate
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  bool CanFocus() override;
  bool OnCaptureLost() override;
  void OnPaint(gfx::Canvas* canvas) override;
  bool ShouldDescendIntoChildForEventHandling(widget::Widget* child,
                                              const gfx::Point& location) override;
  // Overridden from ui::EventHandler:
  void OnKeyEvent(ui::KeyEvent* event) override;
  void OnMouseEvent(ui::MouseEvent* event) override;
  void OnScrollEvent(ui::ScrollEvent* event) override;
  void OnTouchEvent(ui::TouchEvent* event) final;
  void OnGestureEvent(ui::GestureEvent* event) override;

  // Painting ------------------------------------------------------------------
  // Responsible for calling Paint() on child Views. Override to control the
  // order child Views are painted.
  virtual void PaintChildren(gfx::Canvas* canvas, const CullSet& cull_set);

  // Override to paint a background before any content is drawn. Typically this
  // is done if you are satisfied with a default OnPaint handler but wish to
  // supply a different background.
  virtual void OnPaintBackground(gfx::Canvas* canvas);

  // Override to paint a border not specified by SetBorder().
  virtual void OnPaintBorder(gfx::Canvas* canvas);

  virtual void OnVisibleBoundsChanged();
  virtual void OnEnabledChanged();
  virtual void OnVisibilityChanged(bool is_visible);

  View* parent_;
  Views children_;
  gfx::Rect bounds_;
  bool visible_;
  bool enabled_;
  bool focusable_;
  std::string name_;

  scoped_ptr<Background> background_;
  scoped_ptr<Border> border_;
  scoped_ptr<widget::Widget> widget_;
  ObserverList<ViewEventObserver> event_observers_;
  DISALLOW_COPY_AND_ASSIGN(View);
};
}  // namespace views
}  // namespace azer
