#include "azer/ui/win/window.h"

#include "ui/events/event_target_iterator.h"

#include "azer/ui/win/window_observer.h"
#include "azer/ui/win/client/screen_position_client.h"
#include "azer/ui/win/client/event_client.h"

namespace azer {
namespace win {

Window::Window()
    : parent_(NULL)
    , host_(NULL)
    , delegate_(NULL)
    , layer_(NULL)
    , visible_(true)
    , user_data_(NULL)
    , ignore_events_(false) {
}

Window::~Window() {
}

Window* Window::GetRootWindow() {
  return const_cast<Window*>(
      static_cast<const Window*>(this)->GetRootWindow());
}

const Window* Window::GetRootWindow() const {
  return IsRootWindow() ? this : parent_ ? parent_->GetRootWindow() : NULL;
}

WindowTreeHost* Window::GetHost() {
  return const_cast<WindowTreeHost*>(const_cast<const Window*>(this)->
      GetHost());
}

const WindowTreeHost* Window::GetHost() const {
  const Window* root_window = GetRootWindow();
  return root_window ? root_window->host_ : NULL;
}


gfx::Rect Window::GetBoundsInRootWindow() const {
  // TODO(beng): There may be a better way to handle this, and the existing code
  //             is likely wrong anyway in a multi-display world, but this will
  //             do for now.
  if (!GetRootWindow())
    return bounds();
  gfx::Point origin = bounds().origin();
  ConvertPointToTarget(parent_, GetRootWindow(), &origin);
  return gfx::Rect(origin, bounds().size());
}

gfx::Rect Window::GetBoundsInScreen() const {
  gfx::Rect bounds(GetBoundsInRootWindow());
  const Window* root = GetRootWindow();
  if (root) {
    ScreenPositionClient* screen_position_client = GetScreenPositionClient(root);
    if (screen_position_client) {
      gfx::Point origin = bounds.origin();
      screen_position_client->ConvertPointToScreen(root, &origin);
      bounds.set_origin(origin);
    }
  }
  return bounds;
}

// static
void Window::ConvertPointToTarget(const Window* source,
                                  const Window* target,
                                  gfx::Point* point) {
  if (!source)
    return;
  if (source->GetRootWindow() != target->GetRootWindow()) {
    ScreenPositionClient* source_client = 
        GetScreenPositionClient(source->GetRootWindow());
    // |source_client| can be NULL in tests.
    if (source_client)
      source_client->ConvertPointToScreen(source, point);

    ScreenPositionClient* target_client =
        GetScreenPositionClient(target->GetRootWindow());
    // |target_client| can be NULL in tests.
    if (target_client)
      target_client->ConvertPointFromScreen(target, point);
  } else if ((source != target) && (!source->layer() || !target->layer())) {
    if (!source->layer()) {
      gfx::Vector2d offset_to_layer;
      source = source->GetAncestorWithLayer(&offset_to_layer);
      *point += offset_to_layer;
    }
    if (!target->layer()) {
      gfx::Vector2d offset_to_layer;
      target = target->GetAncestorWithLayer(&offset_to_layer);
      *point -= offset_to_layer;
    }
    ::ui::Layer::ConvertPointToLayer(source->layer(), target->layer(), point);
  } else {
    ::ui::Layer::ConvertPointToLayer(source->layer(), target->layer(), point);
  }
}

int64 Window::SetPropertyInternal(const void* key,
                                  const char* name,
                                  PropertyDeallocator deallocator,
                                  int64 value,
                                  int64 default_value) {
  int64 old = GetPropertyInternal(key, default_value);
  if (value == default_value) {
    prop_map_.erase(key);
  } else {
    Value prop_value;
    prop_value.name = name;
    prop_value.value = value;
    prop_value.deallocator = deallocator;
    prop_map_[key] = prop_value;
  }
  FOR_EACH_OBSERVER(WindowObserver, observers_,
                    OnWindowPropertyChanged(this, key, old));
  return old;
}

int64 Window::GetPropertyInternal(const void* key,
                                  int64 default_value) const {
  std::map<const void*, Value>::const_iterator iter = prop_map_.find(key);
  if (iter == prop_map_.end())
    return default_value;
  return iter->second.value;
}

// static
void Window::ConvertRectToTarget(const Window* source,
                                 const Window* target,
                                 gfx::Rect* rect) {
  DCHECK(rect);
  gfx::Point origin = rect->origin();
  ConvertPointToTarget(source, target, &origin);
  rect->set_origin(origin);
}

const Window* Window::GetAncestorWithLayer(gfx::Vector2d* offset) const {
  for (const Window* window = this; window; window = window->parent()) {
    if (window->layer())
      return window;
    if (offset)
      *offset += window->bounds().OffsetFromOrigin();
  }
  if (offset)
    *offset = gfx::Vector2d();
  return NULL;
}

scoped_ptr< ::ui::EventTargeter>
Window::SetEventTargeter(scoped_ptr< ::ui::EventTargeter> targeter) {
  scoped_ptr< ::ui::EventTargeter> old_targeter = targeter_.Pass();
  targeter_ = targeter.Pass();
  return old_targeter.Pass();
}

bool Window::CanAcceptEvent(const ::ui::Event& event) {
  return true;
}

::ui::EventTarget* Window::GetParentTarget() {
  if (IsRootWindow()) {
    return GetEventClient(this)->GetToplevelEventTarget();
  }
  return parent_;
}

scoped_ptr< ::ui::EventTargetIterator> Window::GetChildIterator() const {
  return scoped_ptr< ::ui::EventTargetIterator>(
      new ::ui::EventTargetIteratorImpl<Window>(children()));
}

::ui::EventTargeter* Window::GetEventTargeter() {
  return targeter_.get();
}

void Window::ConvertEventToTarget(::ui::EventTarget* target,
                                  ::ui::LocatedEvent* event) {
  event->ConvertLocationToTarget(this,
                                 static_cast<Window*>(target));
}

void Window::AddChild(Window* child) {
  child->parent_ = this;
  children_.push_back(child);
}

void Window::RemoveChild(Window* child) {
  Windows::iterator i = std::find(children_.begin(), children_.end(), child);
  DCHECK(i != children_.end());
  children_.erase(i);
}

bool Window::Contains(const Window* other) const {
  for (const Window* parent = other; parent; parent = parent->parent_) {
    if (parent == this)
      return true;
  }
  return false;
}

}  // namespace win
}  // namespace azer
