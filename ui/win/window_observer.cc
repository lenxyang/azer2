#include "azer/ui/win/window_event_observer.h"
#include "azer/ui/win/window.h"

namespace azer {
namespace win {

WindowEventDispatcher::WindowEventDispatcher()
    : observing_(0) {
}

WindowEventDispatcher::~WindowEventDispatcher() {
}

void WindowEventDispatcher::OnObservingWindow(Window* window) {
}

void WindowEventDispatcher::OnUnobservingWindow(Window* window) {
}
}  // namespace win
}  // namespace azer
