#include "azer/ui/win/window_event_dispatcher.h"

namespace azer {
namespace win {
WindowEventDispatcher::WindowEventDispatcher(WindowTreeHost* host)
    : host_(host) {
}

WindowEventDispatcher::WindowEventDispatcher() {
}

void WindowEventDispatcher::OnEventProcessingStarted(ui::Event* event) {
}
}  // namespace win
}  // namespace azer
