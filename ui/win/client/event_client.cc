#include "azer/ui/win/client/event_client.h"

#include "ui/aura/window_property.h"
#include "azer/ui/win/window.h"

DECLARE_WINDOW_PROPERTY_TYPE(azer::win::EventClient*)

namespace azer {
namespace win {
DEFINE_WINDOW_PROPERTY_KEY(EventClient*, kRootWindowEventClientKey, NULL);

void SetEventClient(Window* root_window, EventClient* client) {
  DCHECK_EQ(root_window->GetRootWindow(), root_window);
  root_window->SetProperty(kRootWindowEventClientKey, client);
}

EventClient* GetEventClient(const Window* root_window) {
  if (root_window)
    DCHECK_EQ(root_window->GetRootWindow(), root_window);
    return root_window ?
        root_window->GetProperty(kRootWindowEventClientKey) : NULL;
}

}  // namespace win
}  // namespace azer
