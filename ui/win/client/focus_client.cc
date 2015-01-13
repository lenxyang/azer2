#include "azer/ui/win/client/focus_client.h"

#include "azer/ui/win/window_property.h"
#include "azer/ui/win/window.h"

DECLARE_WINDOW_PROPERTY_TYPE(azer::win::FocusChangeObserver*)
DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(AURA_EXPORT, azer::win::Window*)
DECLARE_WINDOW_PROPERTY_TYPE(azer::win::FocusClient*)

namespace azer {
namespace win {

DEFINE_LOCAL_WINDOW_PROPERTY_KEY(
    FocusChangeObserver*, kFocusChangeObserverKey, NULL);

FocusChangeObserver* GetFocusChangeObserver(Window* window) {
  return window ? window->GetProperty(kFocusChangeObserverKey) : NULL;
}

void SetFocusChangeObserver(Window* window,
                            FocusChangeObserver* focus_change_observer) {
  window->SetProperty(kFocusChangeObserverKey, focus_change_observer);
}

DEFINE_WINDOW_PROPERTY_KEY(FocusClient*, kRootWindowFocusClientKey, NULL);

void SetFocusClient(Window* root_window, FocusClient* client) {
  DCHECK_EQ(root_window->GetRootWindow(), root_window);
  root_window->SetProperty(kRootWindowFocusClientKey, client);
}

FocusClient* GetFocusClient(Window* window) {
  return GetFocusClient(static_cast<const Window*>(window));
}

FocusClient* GetFocusClient(const Window* window) {
  const Window* root_window = window->GetRootWindow();
  return root_window ?
      root_window->GetProperty(kRootWindowFocusClientKey) : NULL;
}
}  // namespace win
}  // namespace azer
