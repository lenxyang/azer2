#include "azer/ui/render_widget.h"

#include "ui/views/focus/focus_manager.h"
#include "ui/views/focus/focus_manager_factory.h"
#include "ui/views/widget/root_view.h"
#include "azer/ui/render_window.h"
#include "azer/ui/render_view.h"

namespace azer {

namespace {
class EventView : public views::internal::RootView {
 public:
  static const char kViewClassName[];
  EventView(views::Widget* widget, RenderWindow* window);

  const char* GetClassName() const;
  void OnFocus() override;
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseMoved(const ui::MouseEvent& event) override;
  bool OnMouseWheel(const ui::MouseWheelEvent& event) override;
  void OnMouseCaptureLost() override;
  // bool OnKeyPressed(const ui::KeyEvent& event) override;
  // bool OnKeyReleased(const ui::KeyEvent& event) override;
 private:
  RenderWindow* window_;
  DISALLOW_COPY_AND_ASSIGN(EventView);
};  

const char EventView::kViewClassName[] = "nelf::EventView::EventView";
EventView::EventView(views::Widget* widget, RenderWindow* window)
    : RootView(widget),
      window_(window) {
  SetFocusBehavior(views::View::FocusBehavior::ALWAYS);
}

const char* EventView::GetClassName() const {
  return kViewClassName;
}

void EventView::OnFocus() {
  views::View::OnFocus();
}

bool EventView::OnMousePressed(const ui::MouseEvent& event) {
  window_->GetView()->RequestFocus();
  RootView::OnMousePressed(event);
  return window_->GetView()->OnMousePressed(event);
}

bool EventView::OnMouseDragged(const ui::MouseEvent& event) {
  RootView::OnMouseDragged(event);
  return window_->GetView()->OnMouseDragged(event);
}
void EventView::OnMouseReleased(const ui::MouseEvent& event) {
  RootView::OnMouseReleased(event);
  window_->GetView()->OnMouseReleased(event);
}

void EventView::OnMouseMoved(const ui::MouseEvent& event) {
  RootView::OnMouseMoved(event);
  window_->GetView()->OnMouseMoved(event);
}

bool EventView::OnMouseWheel(const ui::MouseWheelEvent& event) {
  RootView::OnMouseWheel(event);
  return window_->GetView()->OnMouseWheel(event);;
}

void EventView::OnMouseCaptureLost() {
  RootView::OnMouseCaptureLost();
  window_->GetView()->OnMouseCaptureLost();
}
}  // namespace

RenderWindowWidget::RenderWindowWidget(RenderWindow* window) 
    : window_(window) {
}

views::internal::RootView* RenderWindowWidget::CreateRootView() {
  return new EventView(this, window_);
}

void RenderWindowWidget::OnNativeFocus() {
  window_->GetView()->RequestFocus();
  views::Widget::OnNativeFocus();
}

}  // namespace azer
