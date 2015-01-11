#pragma once

#include "base/basictypes.h"
#include "ui/events/event_target.h"
#include "azer/base/export.h"

namespace azer {
namespace win {

class Window;

class AZER_EXPORT EventClient {
 public:
  virtual bool CanProcessEventsWithinSubtree(const Window* window) const = 0;
  virtual ui::EventTarget* GetToplevelEventTarget() = 0;
 protected:
  virtual ~EventClient() {}
};

AZER_EXPORT void SetEventClient(Window* root_window, EventClient* client);
AZER_EXPORT EventClient* GetEventClient(const Window* root_window);
}  // namespace win
}  // namespace azer
