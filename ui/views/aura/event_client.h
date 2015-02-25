#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/scoped_observer.h"
#include "azer/ui/aura/client/event_client.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/views/views_export.h"

namespace views {

class RootView;

class VIEWS_EXPORT EventClient : public aura::client::EventClient {
 public:
  EventClient(RootView* root_view);
  bool CanProcessEventsWithinSubtree(const aura::Window* window) const override;

  ui::EventTarget* GetToplevelEventTarget() override;
 private:
  RootView* root_view_;
  DISALLOW_COPY_AND_ASSIGN(EventClient);
};
}  // namespace views
