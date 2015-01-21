#include "azer/ui/views/root_view.h"

#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "azer/ui/widget/widget_tree_host.h"
#include "azer/ui/widget/widget.h"

namespace azer {
namespace views {

RootView::RootView(const gfx::Rect& bounds) {
  host_.reset(widget::WidgetTreeHost::Create(bounds));
  widget_.reset(host_->root());
  widget_->SetDelegate(this);
  SetName("root");
  SetBounds(0, 0, bounds.width(), bounds.height());
}

RootView::~RootView() {
  widget_.release();
}

void RootView::Show() {
  DCHECK(host_.get());
  host_->Show();
}

void RootView::Hide() {
  DCHECK(host_.get());
  host_->Hide();
}

void RootView::OnPaint(gfx::Canvas* canvas) {
  TRACE_EVENT1("views", "RootView::OnPaint", "class", GetClassName());
  View::OnPaint(canvas);
}

widget::WidgetTreeHost* RootView::GetWidgetTreeHost() { 
  DCHECK(host_.get());
  return host_.get();
}

bool RootView::OnMousePressed(const ui::MouseEvent& event) {
  return View::OnMousePressed(event);
}

bool RootView::OnMouseDragged(const ui::MouseEvent& event) {
  return View::OnMouseDragged(event);
}

void RootView::OnMouseReleased(const ui::MouseEvent& event) {
  View::OnMouseReleased(event);
}

void RootView::OnMouseMoved(const ui::MouseEvent& event) {
  View::OnMouseMoved(event);
}

void RootView::OnMouseEntered(const ui::MouseEvent& event) {
  View::OnMouseEntered(event);
}

void RootView::OnMouseExited(const ui::MouseEvent& event) {
  View::OnMouseExited(event);
}

}  // namespace views
}  // namespace azer
