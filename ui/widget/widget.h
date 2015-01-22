#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/events/event_target.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/base/export.h"
#include "azer/ui/compositor/layer_delegate.h"


namespace gfx {
class Canvas;
}  // namespace gfx

namespace azer {

namespace compositor {
class Layer;
}  // namespace compositor

namespace widget {

class WidgetDelegate;
class WidgetObserver;
class WidgetTreeHost;

// Defined in window_property.h (which we do not include)
template<typename T>
struct WidgetProperty;

class AZER_EXPORT Widget : public compositor::LayerDelegate 
                         , public ::ui::EventTarget {
 public:
  typedef std::vector<Widget*> Widgets;
  enum WidgetType {
    kRoot,
    kNoDraw,
    kCanvas,
    k3DRenderer,
    kBitmap,
  };

  Widget(WidgetType type, Widget* parent, int id=-1);
  Widget(Widget* parent, int id = -1);
  Widget(WidgetTreeHost* host);
  virtual ~Widget();

  WidgetTreeHost* GetHost();
  const WidgetTreeHost* GetHost() const; 

  Widget* GetRootWidget();
  const Widget* GetRootWidget() const;
  compositor::Layer* layer();
  const compositor::Layer* layer() const;

  void SetName(const std::string& name);
  const std::string& name() const;
  WidgetType type() const { return layer_type_;}

  void Hide();
  void Show();
  void SetVisible(bool visible);
  bool IsVisible() const { return visible_;}

  void SetCapture();
  void ReleaseCapture();
  bool HasCapture();

  // Add/remove observer.
  void AddObserver(WidgetObserver* observer);
  void RemoveObserver(WidgetObserver* observer);
  bool HasObserver(WidgetObserver* observer);

  // Sets the |value| of the given window |property|. Setting to the default
  // value (e.g., NULL) removes the property. The caller is responsible for the
  // lifetime of any object set as a property on the Widget.
  template<typename T>
  void SetProperty(const WidgetProperty<T>* property, T value);

  // Returns the value of the given window |property|.  Returns the
  // property-specific default value if the property was not previously set.
  template<typename T>
  T GetProperty(const WidgetProperty<T>* property) const;

  // Sets the |property| to its default value. Useful for avoiding a cast when
  // setting to NULL.
  template<typename T>
  void ClearProperty(const WidgetProperty<T>* property);

  // Type of a function to delete a property that this window owns.
  typedef void (*PropertyDeallocator)(int64 value);

  void SetBounds(const gfx::Rect& bounds);
  const gfx::Rect& bounds() const { return bounds_;}

  gfx::Rect GetBoundsInRootWidget() const;
  gfx::Rect GetBoundsInScreen() const;

  void set_ignore_events(bool ignore) { ignore_events_ = ignore;}
  bool ignore_events() const { return ignore_events_;}
  
  void SetDelegate(WidgetDelegate* delegate);
  WidgetDelegate* delegate() { return delegate_;}

  // Sets a new event-targeter for the window, and returns the previous
  // event-targeter.
  scoped_ptr<ui::EventTargeter> SetEventTargeter(
      scoped_ptr<ui::EventTargeter> targeter);

  Widget* parent() { return parent_;}
  const Widget* parent() const { return parent_;}
  void AddChild(Widget* widget);
  void RemoveChild(Widget* widget);
  bool Contains(const Widget* widget) const;
  const Widgets& children() const { return children_;}

  // Stacks the specified child of this Widget at the front of the z-order.
  void StackChildAtTop(Widget* child);

  // Stacks |child| above |target|.  Does nothing if |child| is already above
  // |target|.  Does not stack on top of windows with NULL layer delegates,
  // see WidgetTest.StackingMadrigal for details.
  void StackChildAbove(Widget* child, Widget* target);

  // Stacks the specified child of this window at the bottom of the z-order.
  void StackChildAtBottom(Widget* child);

  // Stacks |child| below |target|. Does nothing if |child| is already below
  // |target|.
  void StackChildBelow(Widget* child, Widget* target);

  void SchedulePaint();
  void SchedulePaintInRect(const gfx::Rect& r);

  // Converts |point| from |source|'s coordinates to |target|'s. If |source| is
  // NULL, the function returns without modifying |point|. |target| cannot be
  // NULL.
  static void ConvertPointToTarget(const Widget* source,
                                   const Widget* target,
                                   gfx::Point* point);
  static void ConvertRectToTarget(const Widget* source,
                                  const Widget* target,
                                  gfx::Rect* rect);
  // Overridden from ui::EventTarget:

  // A convenience function which calls HitTestRect() with a rect of size
  // 1x1 and an origin of |point|. |point| is in the local coordinate space
  // of |this|.
  bool HitTestPoint(const gfx::Point& point) const;

  // Returns true if |rect| intersects this view's bounds. |rect| is in the
  // local coordinate space of |this|.
  bool HitTestRect(const gfx::Rect& rect) const;
 protected:
  bool CanAcceptEvent(const ui::Event& event) override;
  EventTarget* GetParentTarget() override;
  scoped_ptr<ui::EventTargetIterator> GetChildIterator() const override;
  ui::EventTargeter* GetEventTargeter() override;
 public:
  // access by WidgetTargeter
  void ConvertEventToTarget(ui::EventTarget* target,
                            ui::LocatedEvent* event) override;
 protected:
    // Called by the public {Set,Get,Clear}Property functions.
  int64 SetPropertyInternal(const void* key,
                            const char* name,
                            PropertyDeallocator deallocator,
                            int64 value,
                            int64 default_value);
  int64 GetPropertyInternal(const void* key, int64 default_value) const;

  // Used when stacking windows.
  enum StackDirection {
    STACK_ABOVE,
    STACK_BELOW
  };
  void StackChildRelativeTo(Widget* child, Widget* target, StackDirection direction);
  // Invoked from StackChildRelativeTo() to stack the layers appropriately
  // when stacking |child| relative to |target|.
  void StackChildLayerRelativeTo(Widget* child, Widget* target, 
                                 StackDirection direction);
  void OnStackingChanged();

  void NotifyAddedToRootWidget();
  void NotifyRemovingFromRootWidget();
  void NotifyWidgetVisiblityChanged(Widget* widget, bool visible);
  bool NotifyWidgetVisibilityChangedAtReceiver(Widget* target, bool visible);
  bool NotifyWidgetVisibilityChangedDown(Widget* target, bool visible);
  void NotifyWidgetVisibilityChangedUp(Widget* target, bool visible);
 protected:
  // compositor::LayerDelegate
  void OnPaintLayer(gfx::Canvas* canvas) override;
  void Init();
  compositor::Layer* CreateLayerByType();

  bool IsRootWidget() { return parent_ == NULL;}

  int64 id_;
  WidgetTreeHost* host_;
  Widget* parent_;
  Widgets children_;
  WidgetDelegate* delegate_;
  bool ignore_events_;
  bool visible_;

  WidgetType layer_type_;
  compositor::Layer* layer_;
  scoped_ptr<ui::EventTargeter> targeter_;
  
  gfx::Rect bounds_;

  ObserverList<WidgetObserver, true> observers_;

  // Value struct to keep the name and deallocator for this property.
  // Key cannot be used for this purpose because it can be char* or
  // WidgetProperty<>.
  struct Value {
    const char* name;
    int64 value;
    PropertyDeallocator deallocator;
  };

  std::map<const void*, Value> prop_map_;

  friend class WidgetTreeHost;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};

}  // namespace widget
}  // namespace azer


