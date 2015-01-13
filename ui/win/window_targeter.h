#pragma once

#include "base/basictypes.h"
#include "ui/events/event_targeter.h"
#include "azer/base/export.h"

namespace azer {
namespace win {

class Window;

class WindowTargeter : public ::ui::EventTargeter {
 public:
  WindowTargeter();
  ~WindowTargeter() override;

  ::ui::EventTarget* FindTargetForEvent(::ui::EventTarget* root,
                                        ::ui::Event* event) override;
 private:
  Window* FindTargetForKeyEvent(Window* window, const ::ui::KeyEvent& key);
  DISALLOW_COPY_AND_ASSIGN(WindowTargeter);
};
}  // namespace win
}  // namespace azer
