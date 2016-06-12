#include "azer/ui/render_view.h"

#include "ui/views/widget/widget.h"
#include "azer/ui/event_listener.h"
#include "azer/ui/render_delegate.h"
#include "azer/ui/render_subwindow.h"
#include "azer/ui/render_loop.h"

namespace azer {
const char RenderView::kViewClassName[] = "azer::RenderView";
RenderView::RenderView(RenderLoop* loop, RenderDelegate* delegate)
    : delegate_(delegate),
      window_(NULL),
      loop_(loop) {
  SetFocusBehavior(views::View::FocusBehavior::ALWAYS);
}

RenderView::~RenderView() {
  if (window_) {
    window_->GetWidget()->RemoveObserver(this);
    window_->GetWidget()->Close();
  }
}

const char* RenderView::GetClassName() const {
  return kViewClassName;
}

void RenderView::AddEventListener(EventListener* item) {
  DCHECK(!ContainsEventListener(item));
  listener_.push_back(item);
}

bool RenderView::RemoveEventListener(EventListener* item) {
  auto iter = std::find(listener_.begin(), listener_.end(), item);
  if (iter != listener_.end()) {
    listener_.erase(iter);
    return true;
  } else {
    return false;
  }
}

bool RenderView::ContainsEventListener(EventListener* item) {
  auto iter = std::find(listener_.begin(), listener_.end(), item);
  if (iter != listener_.end()) {
    return true;
  } else {
    return false;
  }
}

void RenderView::OnFocus() {
  views::View::OnFocus();
}

void RenderView::ViewHierarchyChanged(
    const ViewHierarchyChangedDetails& details) {
  if (details.is_add && details.child == this) {
    if (!window_)
      CreateRenderSubWindow();
  }
}

void RenderView::SetSampleDesc(const azer::SampleDesc& desc) { 
  sample_desc_ = desc; 
}

void RenderView::CreateRenderSubWindow() {
  DCHECK(delegate_);
  DCHECK(loop_);
  DCHECK(!window_);
  window_ = new RenderSubWindow(this, delegate_);
  window_->Init(loop_);
  window_->GetWidget()->AddObserver(this);
  views::FocusManager* fmgr = GetWidget()->GetFocusManager();
  DCHECK(fmgr);
  fmgr->AddFocusChangeListener(this);
}

void RenderView::VisibilityChanged(views::View* starting_from, bool is_visible) {
  if (is_visible) {
    window_->GetWidget()->Show();
  } else {
    window_->GetWidget()->Hide();
  }
}

void RenderView::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  gfx::Rect bounds = this->bounds();
  bounds = std::move(views::View::ConvertRectToWidget(bounds));
  window_->GetWidget()->SetBounds(bounds);
}

void RenderView::OnWidgetDestroying(views::Widget* widget) {
  GetFocusManager()->RemoveFocusChangeListener(this);
  loop_->RemoveRenderView(this);
  window_ = NULL;
}

// hande message
bool RenderView::OnMousePressed(const ui::MouseEvent& event) {
  for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
    (*iter)->OnMousePressed(event);
  }
  return true;
}

bool RenderView::OnMouseDragged(const ui::MouseEvent& event) {
  for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
    (*iter)->OnMouseDragged(event);
  }
  return true;
}
void RenderView::OnMouseReleased(const ui::MouseEvent& event) {
  for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
    (*iter)->OnMouseReleased(event);
  }
}

void RenderView::OnMouseMoved(const ui::MouseEvent& event) {
  for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
    (*iter)->OnMouseMoved(event);
  }
}

bool RenderView::OnKeyPressed(const ui::KeyEvent& event) {
  for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
    (*iter)->OnKeyPressed(event);
  }
  return true;
}

bool RenderView::OnKeyReleased(const ui::KeyEvent& event) {
  for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
    (*iter)->OnKeyReleased(event);
  }
  return true;
}

bool RenderView::OnMouseWheel(const ui::MouseWheelEvent& event) {
  for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
    (*iter)->OnMouseWheel(event);
  }
  return true;
}

void RenderView::OnMouseCaptureLost() {
  for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
    (*iter)->OnMouseCaptureLost();
  }
}

void RenderView::OnWillChangeFocus(View* focused_before, View* focused_now) {
}

void RenderView::OnDidChangeFocus(View* focused_before, View* focused_now) {
  if (focused_before == this) {
    for (auto iter = listener_.begin(); iter != listener_.end(); ++iter) {
      (*iter)->OnLostFocus();
    }
  }
}
}  // namespace azer
