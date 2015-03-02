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
#include "ui/accessibility/ax_enums.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/drop_target_event.h"
#include "ui/base/dragdrop/os_exchange_data.h"

#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/views/views_export.h"

using ui::OSExchangeData;

namespace ui {
struct AXViewState;
class NativeTheme;
class TextInputClient;
class ThemeProvider;
}

namespace aura {
class Window;
}  // namespace aura

namespace views {

class Background;
class Border;
class FocusManager;
class FocusTraversable;
class InputMethod;
class Widget;

namespace internal {
class RootView;
}  // namespace internal

class VIEWS_EXPORT View : public aura::WindowDelegate, 
                          public ui::AcceleratorTarget {
 public:
  typedef std::vector<View*> Views;
  View();
  virtual ~View();

  Widget* GetWidget();
  const Widget* GetWidget() const;

  void AddChildView(View* view);
  void RemoveChildView(View* view);
  void RemoveAllChildViews();
  bool Contains(const View* view) const;
  bool has_children() const { return child_count() > 0;}
  int32 child_count() const { return static_cast<int32>(children_.size());}
  View* child_at(int32 index) { return children_.at(index);}
  View* parent() { return parent_;}
  const View* parent() const { return parent_;}

  int id() const;
  void set_id(int id);

  // A group id is used to tag views which are part of the same logical group.
  // Focus can be moved between views with the same group using the arrow keys.
  // Groups are currently used to implement radio button mutual exclusion.
  // The group id is immutable once it's set.
  void SetGroup(int gid);
  // Returns the group id of the view, or -1 if the id is not set yet.
  int GetGroup() const;

  // If this returns true, the views from the same group can each be focused
  // when moving focus with the Tab/Shift-Tab key.  If this returns false,
  // only the selected view from the group (obtained with
  // GetSelectedViewForGroup()) is focused.
  virtual bool IsGroupFocusTraversable() const;

  // Fills |views| with all the available views which belong to the provided
  // |group|.
  void GetViewsInGroup(int group, Views* views);

  // Returns the View that is currently selected in |group|.
  // The default implementation simply returns the first View found for that
  // group.
  virtual View* GetSelectedViewForGroup(int group);

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

  // Accelerators --------------------------------------------------------------

  // Sets a keyboard accelerator for that view. When the user presses the
  // accelerator key combination, the AcceleratorPressed method is invoked.
  // Note that you can set multiple accelerators for a view by invoking this
  // method several times. Note also that AcceleratorPressed is invoked only
  // when CanHandleAccelerators() is true.
  virtual void AddAccelerator(const ui::Accelerator& accelerator);

  // Removes the specified accelerator for this view.
  virtual void RemoveAccelerator(const ui::Accelerator& accelerator);

  // Removes all the keyboard accelerators for this view.
  virtual void ResetAccelerators();

  // Overridden from AcceleratorTarget:
  bool AcceleratorPressed(const ui::Accelerator& accelerator) override;

  // Returns whether accelerators are enabled for this view. Accelerators are
  // enabled if the containing widget is visible and the view is enabled() and
  // IsDrawn()
  bool CanHandleAccelerators() const override;

  // Focus ---------------------------------------------------------------------

  // Returns whether this view currently has the focus.
  virtual bool HasFocus() const;  
  
  // Returns the view that should be selected next when pressing Tab.
  View* GetNextFocusableView();
  const View* GetNextFocusableView() const;

  // Returns the view that should be selected next when pressing Shift-Tab.
  View* GetPreviousFocusableView();

  // Sets the component that should be selected next when pressing Tab, and
  // makes the current view the precedent view of the specified one.
  // Note that by default views are linked in the order they have been added to
  // their container. Use this method if you want to modify the order.
  // IMPORTANT NOTE: loops in the focus hierarchy are not supported.
  void SetNextFocusableView(View* view);

  // Sets whether this view is capable of taking focus. It will clear focus if
  // the focused view is set to be non-focusable.
  // Note that this is false by default so that a view used as a container does
  // not get the focus.
  void SetFocusable(bool focusable);

  // Returns true if this view is |focusable_|, |enabled_| and drawn.
  bool IsFocusable() const;

  // Return whether this view is focusable when the user requires full keyboard
  // access, even though it may not be normally focusable.
  bool IsAccessibilityFocusable() const;

  // Set whether this view can be made focusable if the user requires
  // full keyboard access, even though it's not normally focusable. It will
  // clear focus if the focused view is set to be non-focusable.
  // Note that this is false by default.
  void SetAccessibilityFocusable(bool accessibility_focusable);

  // Convenience method to retrieve the FocusManager associated with the
  // Widget that contains this view.  This can return NULL if this view is not
  // part of a view hierarchy with a Widget.
  virtual FocusManager* GetFocusManager();
  virtual const FocusManager* GetFocusManager() const;

  // Request keyboard focus. The receiving view will become the focused view.
  virtual void RequestFocus();

  // Invoked when a view is about to be requested for focus due to the focus
  // traversal. Reverse is this request was generated going backward
  // (Shift-Tab).
  virtual void AboutToRequestFocusFromTabTraversal(bool reverse) {}

  // Invoked when a key is pressed before the key event is processed (and
  // potentially eaten) by the focus manager for tab traversal, accelerators and
  // other focus related actions.
  // The default implementation returns false, ensuring that tab traversal and
  // accelerators processing is performed.
  // Subclasses should return true if they want to process the key event and not
  // have it processed as an accelerator (if any) or as a tab traversal (if the
  // key event is for the TAB key).  In that case, OnKeyPressed will
  // subsequently be invoked for that event.
  virtual bool SkipDefaultKeyEventProcessing(const ui::KeyEvent& event);

  // Subclasses that contain traversable children that are not directly
  // accessible through the children hierarchy should return the associated
  // FocusTraversable for the focus traversal to work properly.
  virtual FocusTraversable* GetFocusTraversable();

  // Subclasses that can act as a "pane" must implement their own
  // FocusTraversable to keep the focus trapped within the pane.
  // If this method returns an object, any view that's a direct or
  // indirect child of this view will always use this FocusTraversable
  // rather than the one from the widget.
  virtual FocusTraversable* GetPaneFocusTraversable();

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

  // RTL painting --------------------------------------------------------------

  // This method determines whether the gfx::Canvas object passed to
  // View::Paint() needs to be transformed such that anything drawn on the
  // canvas object during View::Paint() is flipped horizontally.
  //
  // By default, this function returns false (which is the initial value of
  // |flip_canvas_on_paint_for_rtl_ui_|). View subclasses that need to paint on
  // a flipped gfx::Canvas when the UI layout is right-to-left need to call
  // EnableCanvasFlippingForRTLUI().
  bool FlipCanvasOnPaintForRTLUI() const {
    return flip_canvas_on_paint_for_rtl_ui_ ? base::i18n::IsRTL() : false;
  }

  // Enables or disables flipping of the gfx::Canvas during View::Paint().
  // Note that if canvas flipping is enabled, the canvas will be flipped only
  // if the UI layout is right-to-left; that is, the canvas will be flipped
  // only if base::i18n::IsRTL() returns true.
  //
  // Enabling canvas flipping is useful for leaf views that draw an image that
  // needs to be flipped horizontally when the UI layout is right-to-left
  // (views::Button, for example). This method is helpful for such classes
  // because their drawing logic stays the same and they can become agnostic to
  // the UI directionality.
  void EnableCanvasFlippingForRTLUI(bool enable) {
    flip_canvas_on_paint_for_rtl_ui_ = enable;
  }

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


  // RTL positioning -----------------------------------------------------------

  // Methods for accessing the bounds and position of the view, relative to its
  // parent. The position returned is mirrored if the parent view is using a RTL
  // layout.
  //
  // NOTE: in the vast majority of the cases, the mirroring implementation is
  //       transparent to the View subclasses and therefore you should use the
  //       bounds() accessor instead.
  gfx::Rect GetMirroredBounds() const;
  gfx::Point GetMirroredPosition() const;
  int GetMirroredX() const;

  // Given a rectangle specified in this View's coordinate system, the function
  // computes the 'left' value for the mirrored rectangle within this View. If
  // the View's UI layout is not right-to-left, then bounds.x() is returned.
  //
  // UI mirroring is transparent to most View subclasses and therefore there is
  // no need to call this routine from anywhere within your subclass
  // implementation.
  int GetMirroredXForRect(const gfx::Rect& rect) const;

  // Given the X coordinate of a point inside the View, this function returns
  // the mirrored X coordinate of the point if the View's UI layout is
  // right-to-left. If the layout is left-to-right, the same X coordinate is
  // returned.
  //
  // Following are a few examples of the values returned by this function for
  // a View with the bounds {0, 0, 100, 100} and a right-to-left layout:
  //
  // GetMirroredXCoordinateInView(0) -> 100
  // GetMirroredXCoordinateInView(20) -> 80
  // GetMirroredXCoordinateInView(99) -> 1
  int GetMirroredXInView(int x) const;

  // Given a X coordinate and a width inside the View, this function returns
  // the mirrored X coordinate if the View's UI layout is right-to-left. If the
  // layout is left-to-right, the same X coordinate is returned.
  //
  // Following are a few examples of the values returned by this function for
  // a View with the bounds {0, 0, 100, 100} and a right-to-left layout:
  //
  // GetMirroredXCoordinateInView(0, 10) -> 90
  // GetMirroredXCoordinateInView(20, 20) -> 60
  int GetMirroredXWithWidthInView(int x, int w) const;

  virtual void Layout();

  virtual void Show();
  virtual void Hide();

  bool visible() const { return visible_;}
 
  aura::Window* window() { return window_.get();}
  const aura::Window* window() const { return window_.get();}

  internal::RootView* GetRootView() { return root_;}
  const internal::RootView* GetRootView() const { return root_;}

  // Convert a point from a View's coordinate system to that of the screen.
  static void ConvertPointToScreen(const View* src, gfx::Point* point);

  // Convert a point from a View's coordinate system to that of the screen.
  static void ConvertPointFromScreen(const View* dst, gfx::Point* point);

  // The view class name.
  static const char kViewClassName[];

  // Return the receiving view's class name. A view class is a string which
  // uniquely identifies the view class. It is intended to be used as a way to
  // find out during run time if a view can be safely casted to a specific view
  // subclass. The default implementation returns kViewClassName.
  virtual const char* GetClassName() const;

  void InvalidateLayout();

  virtual gfx::Size GetPreferredSize() const;
  
  void OnPaint(gfx::Canvas* canvas) override;
  virtual void OnPaintBackground(gfx::Canvas* canvas);
  virtual void OnPaintBorder(gfx::Canvas* canvas);


  // Size and disposition ------------------------------------------------------

  // Override to be notified when the bounds of the view have changed.
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds);

  // Called when the preferred size of a child view changed.  This gives the
  // parent an opportunity to do a fresh layout if that makes sense.
  virtual void ChildPreferredSizeChanged(View* child) {}

  // Called when the visibility of a child view changed.  This gives the parent
  // an opportunity to do a fresh layout if that makes sense.
  virtual void ChildVisibilityChanged(View* child) {}

  // Invalidates the layout and calls ChildPreferredSizeChanged on the parent
  // if there is one. Be sure to call View::PreferredSizeChanged when
  // overriding such that the layout is properly invalidated.
  virtual void PreferredSizeChanged();

  // Override returning true when the view needs to be notified when its visible
  // bounds relative to the root view may have changed. Only used by
  // NativeViewHost.
  virtual bool GetNeedsNotificationWhenVisibleBoundsChange() const;

  // Notification that this View's visible bounds relative to the root view may
  // have changed. The visible bounds are the region of the View not clipped by
  // its ancestors. This is used for clipping NativeViewHost.
  virtual void OnVisibleBoundsChanged();

  // Override to be notified when the enabled state of this View has
  // changed. The default implementation calls SchedulePaint() on this View.
  virtual void OnEnabledChanged();

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

  // Handle view focus/blur events for this view.
  void Focus();
  void Blur();

  virtual void VisibilityChanged(View* starting_from, bool is_visible);


  // System events -------------------------------------------------------------

  // Called when the UI theme (not the NativeTheme) has changed, overriding
  // allows individual Views to do special cleanup and processing (such as
  // dropping resource caches).  To dispatch a theme changed notification, call
  // Widget::ThemeChanged().
  virtual void OnThemeChanged() {}

  // Used to propagate theme changed notifications from the root view to all
  // views in the hierarchy.
  virtual void PropagateThemeChanged();

  // Tooltips ------------------------------------------------------------------

  // Gets the tooltip for this View. If the View does not have a tooltip,
  // return false. If the View does have a tooltip, copy the tooltip into
  // the supplied string and return true.
  // Any time the tooltip text that a View is displaying changes, it must
  // invoke TooltipTextChanged.
  // |p| provides the coordinates of the mouse (relative to this view).
  virtual bool GetTooltipText(const gfx::Point& p,
                              base::string16* tooltip) const;

  // Returns the location (relative to this View) for the text on the tooltip
  // to display. If false is returned (the default), the tooltip is placed at
  // a default position.
  virtual bool GetTooltipTextOrigin(const gfx::Point& p, gfx::Point* loc) const;

  // See field for description.
  void set_notify_enter_exit_on_child(bool notify) {
    notify_enter_exit_on_child_ = notify;
  }
  bool notify_enter_exit_on_child() const {
    return notify_enter_exit_on_child_;
  }

  // Returns the View's TextInputClient instance or NULL if the View doesn't
  // support text input.
  virtual ui::TextInputClient* GetTextInputClient();

  // Convenience method to retrieve the InputMethod associated with the
  // Widget that contains this view. Returns NULL if this view is not part of a
  // view hierarchy with a Widget.
  virtual InputMethod* GetInputMethod();
  virtual const InputMethod* GetInputMethod() const;

  // Drag and drop -------------------------------------------------------------

  // These are cover methods that invoke the method of the same name on
  // the DragController. Subclasses may wish to override rather than install
  // a DragController.
  // See DragController for a description of these methods.
  virtual int GetDragOperations(const gfx::Point& press_pt);
  virtual void WriteDragData(const gfx::Point& press_pt, OSExchangeData* data);

  // Returns whether we're in the middle of a drag session that was initiated
  // by us.
  bool InDrag();

  virtual bool CanDrop(const ui::OSExchangeData& data);
  virtual void OnDragEntered(const ui::DropTargetEvent& event);
  virtual int OnDragUpdated(const ui::DropTargetEvent& event);
  virtual void OnDragExited();
  virtual int OnPerformDrop(const ui::DropTargetEvent& event);
  virtual void OnDragDone();


  // Modifies |state| to reflect the current accessible state of this view.
  virtual void GetAccessibleState(ui::AXViewState* state) { }

  // Notifies assistive technology that an accessibility event has
  // occurred on this view, such as when the view is focused or when its
  // value changes. Pass true for |send_native_event| except for rare
  // cases where the view is a native control that's already sending a
  // native accessibility event and the duplicate event would cause
  // problems.
  void NotifyAccessibilityEvent(ui::AXEvent event_type,
                                bool send_native_event);

  // Return the cursor that should be used for this view or the default cursor.
  // The event location is in the receiver's coordinate system. The caller is
  // responsible for managing the lifetime of the returned object, though that
  // lifetime may vary from platform to platform. On Windows and Aura,
  // the cursor is a shared resource.
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event);

  // A convenience function which calls HitTestRect() with a rect of size
  // 1x1 and an origin of |point|. |point| is in the local coordinate space
  // of |this|.
  bool HitTestPoint(const gfx::Point& local_point) const;

  // Returns true if |rect| intersects this view's bounds. |rect| is in the
  // local coordinate space of |this|.
  bool HitTestRect(const gfx::Rect& local_rect) const;

  // Returns true if this view or any of its descendants are permitted to
  // be the target of an event.
  virtual bool CanProcessEventsWithinSubtree() const;

  void SchedulePaint();


  // Returns true if the mouse cursor is over |view| and mouse events are
  // enabled.
  bool IsMouseHovered();
 protected:
  // Size and disposition ------------------------------------------------------

  // Call VisibilityChanged() recursively for all children.
  void PropagateVisibilityNotifications(View* from, bool is_visible);

  // Registers/unregisters accelerators as necessary and calls
  // VisibilityChanged().
  void VisibilityChangedImpl(View* starting_from, bool is_visible);

  // Responsible for propagating bounds change notifications to relevant
  // views.
  void BoundsChanged(const gfx::Rect& previous_bounds);

  // Visible bounds notification registration.
  // When a view is added to a hierarchy, it and all its children are asked if
  // they need to be registered for "visible bounds within root" notifications
  // (see comment on OnVisibleBoundsChanged()). If they do, they are registered
  // with every ancestor between them and the root of the hierarchy.
  static void RegisterChildrenForVisibleBoundsNotification(View* view);
  static void UnregisterChildrenForVisibleBoundsNotification(View* view);
  void RegisterForVisibleBoundsNotification();
  void UnregisterForVisibleBoundsNotification();

  // Adds/removes view to the list of descendants that are notified any time
  // this views location and possibly size are changed.
  void AddDescendantToNotify(View* view);
  void RemoveDescendantToNotify(View* view);

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

  bool ProcessMousePressed(const ui::MouseEvent& event);
  bool ProcessMouseDragged(const ui::MouseEvent& event);
  void ProcessMouseReleased(const ui::MouseEvent& event);

  // Invoked when the NativeTheme associated with this View changes.
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) {}
  
  // Overridden from aura::WindowDelegate --------------------------------------
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

  // Accelerators --------------------------------------------------------------

  // Registers this view's keyboard accelerators that are not registered to
  // FocusManager yet, if possible.
  void RegisterPendingAccelerators();

  // Unregisters all the keyboard accelerators associated with this view.
  // |leave_data_intact| if true does not remove data from accelerators_ array,
  // so it could be re-registered with other focus manager
  void UnregisterAccelerators(bool leave_data_intact);


  // when view attached to parent with root.
  void OnAttachedRecusive(internal::RootView* view);
  void OnDetachRecusive();

  void PropagateNativeThemeChanged(const ui::NativeTheme* theme);
  void PropagateMouseEntered(const ui::MouseEvent& event);
  void PropagateMouseExited(const ui::MouseEvent& event);

  virtual void InitAuraWindow(aura::WindowLayerType layer_type);

  scoped_ptr<aura::Window> window_;

  // Attributes ----------------------------------------------------------------

  // The id of this View. Used to find this View.
  // int id_;

  // The group of this view. Some view subclasses use this id to find other
  // views of the same group. For example radio button uses this information
  // to find other radio buttons.
  int group_;

  // Tree operations -----------------------------------------------------------

  // This view's parent.
  View* parent_;

  // This view's children.
  Views children_;

  internal::RootView* root_;

  // Focus ---------------------------------------------------------------------

  // Next view to be focused when the Tab key is pressed.
  View* next_focusable_view_;

  // Next view to be focused when the Shift-Tab key combination is pressed.
  View* previous_focusable_view_;

  // Whether this view can be focused.
  bool focusable_;

  // Whether this view is focusable if the user requires full keyboard access,
  // even though it may not be normally focusable.
  bool accessibility_focusable_;

  // This View's bounds in the parent coordinate system.
  gfx::Rect bounds_;

  // Whether this view is visible.
  bool visible_;

  // Whether this view is enabled.
  bool enabled_;

  // When this flag is on, a View receives a mouse-enter and mouse-leave event
  // even if a descendant View is the event-recipient for the real mouse
  // events. When this flag is turned on, and mouse moves from outside of the
  // view into a child view, both the child view and this view receives
  // mouse-enter event. Similarly, if the mouse moves from inside a child view
  // and out of this view, then both views receive a mouse-leave event.
  // When this flag is turned off, if the mouse moves from inside this view into
  // a child view, then this view receives a mouse-leave event. When this flag
  // is turned on, it does not receive the mouse-leave event in this case.
  // When the mouse moves from inside the child view out of the child view but
  // still into this view, this view receives a mouse-enter event if this flag
  // is turned off, but doesn't if this flag is turned on.
  // This flag is initialized to false.
  bool notify_enter_exit_on_child_;

  bool needs_layout_;

  // Painting ------------------------------------------------------------------

  // Background
  scoped_ptr<Background> background_;

  // Border.
  scoped_ptr<Border> border_;

  // RTL painting --------------------------------------------------------------

  // Indicates whether or not the gfx::Canvas object passed to View::Paint()
  // is going to be flipped horizontally (using the appropriate transform) on
  // right-to-left locales for this View.
  bool flip_canvas_on_paint_for_rtl_ui_;

  // The list of accelerators. List elements in the range
  // [0, registered_accelerator_count_) are already registered to FocusManager,
  // and the rest are not yet.
  scoped_ptr<std::vector<ui::Accelerator> > accelerators_;
  size_t registered_accelerator_count_;

  // Whether or not RegisterViewForVisibleBoundsNotification on the RootView
  // has been invoked.
  bool registered_for_visible_bounds_notification_;

  // List of descendants wanting notification when their visible bounds change.
  scoped_ptr<Views> descendants_to_notify_;

  bool mouse_in_;

  friend class Widget;
  DISALLOW_COPY_AND_ASSIGN(View);
};
}  // namespace views
