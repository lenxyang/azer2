#include "azer/ui/views/widget/aura/focus_client.h"

#include "azer/ui/aura/client/focus_change_observer.h"
#include "azer/ui/aura/window.h"

namespace views {

////////////////////////////////////////////////////////////////////////////////
// FocusClient, public:

FocusClient::FocusClient()
    : focused_window_(NULL),
      observer_manager_(this) {
}

FocusClient::~FocusClient() {
}

////////////////////////////////////////////////////////////////////////////////
// FocusClient, client::FocusClient implementation:

void FocusClient::AddObserver(client::FocusChangeObserver* observer) {
}

void FocusClient::RemoveObserver(client::FocusChangeObserver* observer) {
}

void FocusClient::FocusWindow(Window* window) {
  if (window && !window->CanFocus())
    return;
  if (focused_window_)
    observer_manager_.Remove(focused_window_);
  aura::Window* old_focused_window = focused_window_;
  focused_window_ = window;
  if (focused_window_)
    observer_manager_.Add(focused_window_);

  client::FocusChangeObserver* observer =
      client::GetFocusChangeObserver(old_focused_window);
  if (observer)
    observer->OnWindowFocused(focused_window_, old_focused_window);
  observer = client::GetFocusChangeObserver(focused_window_);
  if (observer)
    observer->OnWindowFocused(focused_window_, old_focused_window);
}

void FocusClient::ResetFocusWithinActiveWindow(Window* window) {
  if (!window->Contains(focused_window_))
    FocusWindow(window);
}

Window* FocusClient::GetFocusedWindow() {
  return focused_window_;
}

////////////////////////////////////////////////////////////////////////////////
// FocusClient, WindowObserver implementation:

void FocusClient::OnWindowDestroying(Window* window) {
  DCHECK_EQ(window, focused_window_);
  FocusWindow(NULL);
}

}  // namespace views
