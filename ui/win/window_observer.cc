#include "azer/ui/win/window_observer.h"
#include "azer/ui/win/window.h"

namespace azer {
namespace win {

WindowObserver::WindowObserver()
    : observing_(0) {
}

WindowObserver::~WindowObserver() {
}

void WindowObserver::OnObservingWindow(Window* window) {
}

void WindowObserver::OnUnobservingWindow(Window* window) {
}
}  // namespace win
}  // namespace azer
