#include "azer/util/interactive/interactive_context.h"

#include "base/logging.h"
#include "azer/util/interactive/interactive_controller.h"
#include "azer/util/interactive/pick_util.h"

namespace azer {
InteractiveContext::InteractiveContext()
    : inactived_(-1),
      dragging_(false) {
}

void InteractiveContext::Active(InteractiveController* controller) {
  int index = GetIndexOf(controller);
  DCHECK_GE(index, 0);
  activated_ = index;
}

void InteractiveContext::AddController(InteractiveController* controller) {
  controllers_.push_back(controller);
}

void InteractiveContext::RemoveController(InteractiveController* controller) {
  int index = GetIndexOf(controller);
  DCHECK_GE(index, 0);
  controllers_.erase(controllers_.begin() + index);
}

int InteractiveContext::GetIndexOf(InteractiveController* controller) const {
  auto i(std::find(controllers_.begin(), controllers_.end(), view));
  return i != controllers_.end() ? static_cast<int>(i - controllers_.begin()) : -1;
}

void InteractiveContext::OnMousePressed(const ui::MouseEvent& event) {
  OnDrawBegin(event);
}

void InteractiveContext::OnMouseDragged(const ui::MouseEvent& event) {
  OnDrag(event);
}

void InteractiveContext::OnMouseReleased(const ui::MouseEvent& event) {
  OnDragEnd(event);
}

void InteractiveContext::OnMouseMoved(const ui::MouseEvent& event) {
  if (activated >= 0) {
    InteractiveController* controller = controllers_[activated_];
    controller->HitTest(event.location());
  }
}

void InteractiveContext::OnMouseCaptureLost() {
  OnDragEnd(event);
}

void InteractiveContext::OnDrawBegin(const ui::MouseEvent& event) {
  if (activated >= 0) {
    InteractiveController* controller = controllers_[activated_];
    int ret = controller->GetPicking(event.location());
    if (ret > 0) {
      drag_ = true;
      controller_->OnDragBegin(event.location());
    }
  }
}

void InteractiveContext::OnDrag(const ui::MouseEvent& event) {
  if (drawing_) {
    InteractiveController* controller = controllers_[activated_];
    controller_->OnDrag(event.location());
  }
}

void InteractiveContext::OnDragEnd(const ui::MouseEvent& event) {
  if (drawing_) {
    InteractiveController* controller = controllers_[activated_];
    controller_->OnDragEnd(event.location());
    drawing_ = false;
  }
}
}  // namespace azer
