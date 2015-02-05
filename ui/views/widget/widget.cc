#include "azer/ui/views/widget/widget.h"

#include "base/logging.h"
#include "azer/ui/views/view.h"

namespace views {

Widget::Widget(View* view)
    : aura::Window(this)
    , view_(view) {
}

Widget::~Widget() {
}

void Widget::OnMouseEvent(ui::MouseEvent* event) {
  view_->OnMouseEvent(event);
}

void Widget::OnScrollEvent(ui::ScrollEvent* event) {
  view_->OnScrollEvent(event);
}

void Widget::OnTouchEvent(ui::TouchEvent* event) {
  view_->OnTouchEvent(event);
}

void Widget::OnGestureEvent(ui::GestureEvent* event) {
  view_->OnGestureEvent(event);
}

}  // namespace views
