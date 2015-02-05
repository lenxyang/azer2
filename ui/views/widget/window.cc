#include "azer/ui/views/widget/window.h"

#include "base/logging.h"
#include "azer/ui/views/view.h"

namespace views {

Window::Window(View* view)
    : aura::Window(this)
    , view_(view) {
}

Window::~Window() {
}

void Window::OnMouseEvent(ui::MouseEvent* event) {
  view_->OnMouseEvent(event);
}

void Window::OnScrollEvent(ui::ScrollEvent* event) {
  view_->OnScrollEvent(event);
}

void Window::OnTouchEvent(ui::TouchEvent* event) {
  view_->OnTouchEvent(event);
}

void Window::OnGestureEvent(ui::GestureEvent* event) {
  view_->OnGestureEvent(event);
}

}  // namespace views
