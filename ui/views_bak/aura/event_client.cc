#include "azer/ui/views/aura/event_client.h"

#include "azer/ui/views/widget/root_view.h"
#include "azer/ui/views/view.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_property.h"
#include "azer/ui/aura/env.h"

DECLARE_WINDOW_PROPERTY_TYPE(views::View*)

namespace views {

DEFINE_WINDOW_PROPERTY_KEY(View*, kAzerView, NULL);

EventClient::EventClient(internal::RootView* root_view) 
    : root_view_(root_view) {
}

EventClient::~EventClient() {
}

bool EventClient::CanProcessEventsWithinSubtree(const aura::Window* window) const {
  View* view = window->GetProperty(kAzerView);
  if (view) {
    return view->CanProcessEventsWithinSubtree();
  } else {
    return true;
  }
}

ui::EventTarget* EventClient::GetToplevelEventTarget() {
  return aura::Env::GetInstance();
}

void VIEWS_EXPORT SetViewInAuraWindowProperty(View* view) {
  view->window()->SetProperty(kAzerView, view);
}
}  // namespace views
