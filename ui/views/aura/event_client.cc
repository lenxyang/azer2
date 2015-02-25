#include "azer/ui/views/aura/event_client.h"

#include "azer/ui/views/root_view.h"
#include "azer/ui/views/view.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/aura/env.h"

namespace views {

DEFINE_WINDOW_PROPERTY_KEY(View*, kAzerView, NULL);

EventClient::EventClient(RootView* root_view) 
    : root_view_(root_view) {
}

EventClient::~EventClient() {
}

bool EventClient::CanProcessEventsWithinSubtree(const aura::Window* window) const {
  View* view = window->GetProperty(kAzerView);
  return view->CanProcessEventsWithinSubtree();
}

ui::EventTarget* EventClient::GetToplevelEventTarget() {
  return aura::Env::GetInstance();
}
}  // namespace views
