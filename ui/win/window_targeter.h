#pragma once

#include "base/basictypes.h"
#include "ui/events/event_targeter.h"
#include "azer/base/export.h"

namespace azer {
namespace ui {
class WindowTargeter : public ui::EventTargeter {
 public:
  WindowTargeter();
  ~WindowTargeter() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(WindowTargeter);
};
}  // namespace ui
}  // namespace azer
