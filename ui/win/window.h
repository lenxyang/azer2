#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "ui/aura/window_property.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/wm/public/window_types.h"

#include "azer/base/export.h"
#include "azer/ui/win/layer_type.h"
#include "azer/ui/compositor/layer.h"

namespace gfx {
class Display;
class Transform;
class Vector2d;
}  // namespace gfx

namespace azer {
namespace win {

class WindowTreeHost;
class WindowDelegate;
class WindowObserver;

// Defined in window_property.h (which we do not include)
template<typename T>
struct WindowProperty;

class AZER_EXPORT Window : public ::ui::EventTarget {
 public:
  typedef std::vector<Window*> Windows;
  Window(WindowDelegate* delegate);
  ~Window() override;

  void Init(WindowLayerType layer_type);

  // A type is used to identify a class of Windows and customize behavior such
  // as event handling and parenting.  This field should only be consumed by the
  // shell -- Aura itself shouldn't contain type-specific logic.
  ui::wm::WindowType type() const { return type_; }
  void SetType(ui::wm::WindowType type);

  WindowDelegate* delegate() { return delegate_; }
  const WindowDelegate* delegate() const { return delegate_; }

  int id() const { return id_;}
  void set_id(int id) { id_ = id;}
  const std::string& name() const { return name_;}
  void SetName(const std::string&  name);

  const ::base::string16& Title() const { return title_;}
  void SetTitle(const ::base::string16& title);

  const gfx::Rect& bounds() const { return bounds_; }

  Window* parent() { return parent_; }
  const Window* parent() const { return parent_; }

  ui::Layer* layer() { return layer_;}
  const ui::Layer* layer() const { return layer_;}

  // Returns the root Window that contains this Window. The root Window is
  // defined as the Window that has a dispatcher. These functions return NULL if
  // the Window is contained in a hierarchy that does not have a dispatcher at
  // its root.
  Window* GetRootWindow();
  const Window* GetRootWindow() const;

  WindowTreeHost* GetHost();
  const WindowTreeHost* GetHost() const;
  void set_host(WindowTreeHost* host) { host_ = host; }
  bool IsRootWindow() const { return !!host_; }

  // The Window does not own this object.
  void set_user_data(void* user_data) { user_data_ = user_data; }
  void* user_data() const { return user_data_; }

    // Changes the visibility of the window.
  void Show();
  void Hide();
  // Returns true if this window and all its ancestors are visible.
  bool IsVisible() const;
  // Returns the visibility requested by this window. IsVisible() takes into
  // account the visibility of the layer and ancestors, where as this tracks
  // whether Show() without a Hide() has been invoked.
  bool TargetVisibility() const { return visible_; }

  // Returns the window's bounds in root window's coordinates.
  gfx::Rect GetBoundsInRootWindow() const;

  // Returns the window's bounds in screen coordinates.
  // How the root window's coordinates is mapped to screen's coordinates
  // is platform dependent and defined in the implementation of the
  // |aura::client::ScreenPositionClient| interface.
  gfx::Rect GetBoundsInScreen() const;

  // Returns the target bounds of the window. If the window's layer is
  // not animating, it simply returns the current bounds.
  gfx::Rect GetTargetBounds() const;

  void SetTransform(const gfx::Transform& transform);

  // Changes the bounds of the window. If present, the window's parent's
  // LayoutManager may adjust the bounds.
  void SetBounds(const gfx::Rect& new_bounds);

  // Sets a new event-targeter for the window, and returns the previous
  // event-targeter.
  scoped_ptr<::ui::EventTargeter> SetEventTargeter(
      scoped_ptr<::ui::EventTargeter> targeter);

  // Sets the |value| of the given window |property|. Setting to the default
  // value (e.g., NULL) removes the property. The caller is responsible for the
  // lifetime of any object set as a property on the Window.
  template<typename T>
      void SetProperty(const WindowProperty<T>* property, T value);

  // Returns the value of the given window |property|.  Returns the
  // property-specific default value if the property was not previously set.
  template<typename T>
      T GetProperty(const WindowProperty<T>* property) const;

  // Sets the |property| to its default value. Useful for avoiding a cast when
  // setting to NULL.
  template<typename T>
      void ClearProperty(const WindowProperty<T>* property);

  // Type of a function to delete a property that this window owns.
  typedef void (*PropertyDeallocator)(int64 value);

  // Converts |point| from |source|'s coordinates to |target|'s. If |source| is
  // NULL, the function returns without modifying |point|. |target| cannot be
  // NULL.
  static void ConvertPointToTarget(const Window* source,
                                   const Window* target,
                                   gfx::Point* point);
  static void ConvertRectToTarget(const Window* source,
                                  const Window* target,
                                  gfx::Rect* rect);

  // Returns true if the |point_in_root| in root window's coordinate falls
  // within this window's bounds. Returns false if the window is detached
  // from root window.
  bool ContainsPointInRoot(const gfx::Point& point_in_root) const;

  // Returns true if relative-to-this-Window's-origin |local_point| falls
  // within this Window's bounds.
  bool ContainsPoint(const gfx::Point& local_point) const;

  // Returns the Window that most closely encloses |local_point| for the
  // purposes of event targeting.
  Window* GetEventHandlerForPoint(const gfx::Point& local_point);

  // Returns the topmost Window with a delegate containing |local_point|.
  Window* GetTopWindowContainingPoint(const gfx::Point& local_point);

  // Returns this window's toplevel window (the highest-up-the-tree anscestor
  // that has a delegate set).  The toplevel window may be |this|.
  Window* GetToplevelWindow();

  // Gets a Window (either this one or a subwindow) containing |local_point|.
  // If |return_tightest| is true, returns the tightest-containing (i.e.
  // furthest down the hierarchy) Window containing the point; otherwise,
  // returns the loosest.  If |for_event_handling| is true, then hit-test masks
  // are honored; otherwise, only bounds checks are performed.
  Window* GetWindowForPoint(const gfx::Point& local_point,
                            bool return_tightest,
                            bool for_event_handling);

  // Returns the first ancestor (starting at |this|) with a layer. |offset| is
  // set to the offset from |this| to the first ancestor with a layer. |offset|
  // may be NULL.
  Window* GetAncestorWithLayer(gfx::Vector2d* offset) {
    return const_cast<Window*>(
        const_cast<const Window*>(this)->GetAncestorWithLayer(offset));
  }
  const Window* GetAncestorWithLayer(gfx::Vector2d* offset) const;

  // Add/remove observer.
  void AddObserver(WindowObserver* observer);
  void RemoveObserver(WindowObserver* observer);
  bool HasObserver(WindowObserver* observer);

  void set_ignore_events(bool ignore_events) { ignore_events_ = ignore_events; }
  bool ignore_events() const { return ignore_events_; }

  // Overridden from ::ui::EventTarget:
  bool CanAcceptEvent(const ::ui::Event& event) override;
  EventTarget* GetParentTarget() override;
  scoped_ptr<ui::EventTargetIterator> GetChildIterator() const override;
  ::ui::EventTargeter* GetEventTargeter() override;
  void ConvertEventToTarget(::ui::EventTarget* target,
                            ::ui::LocatedEvent* event) override;

  // subwindow
  void AddChild(Window* child);
  void RemoveChild(Window* child);
  const Windows& children() const { return children_; }
  bool Contains(const Window* other) const;
 private:
  // Called by the public {Set,Get,Clear}Property functions.
  int64 SetPropertyInternal(const void* key,
                            const char* name,
                            PropertyDeallocator deallocator,
                            int64 value,
                            int64 default_value);
  int64 GetPropertyInternal(const void* key, int64 default_value) const;

  void SetBoundsInternal(const gfx::Rect& bounds);

  void SetVisible(bool visible);

  Window* parent_;
  Windows children_;
  WindowTreeHost* host_;
  ui::wm::WindowType type_;
  WindowDelegate* delegate_;
  
  LayerPtr layer_;
  bool visible_;
  bool ignore_events_;
  void *user_data_;
  gfx::Rect bounds_;

  int id_;
  std::string name_;
  base::string16 title_;

  scoped_ptr<ui::EventTargeter> targeter_;

  ObserverList<WindowObserver, true> observers_;
  // Value struct to keep the name and deallocator for this property.
  // Key cannot be used for this purpose because it can be char* or
  // WindowProperty<>.
  struct Value {
    const char* name;
    int64 value;
    PropertyDeallocator deallocator;
  };

  std::map<const void*, Value> prop_map_;
  DISALLOW_COPY_AND_ASSIGN(Window);
};
}  // namespace win
}  // namespace azer
