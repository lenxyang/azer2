#include "azer/ui/views/root_view.h"

#include "base/logging.h"
#include "azer/ui/widget/widget_tree_host.h"
#include "azer/ui/widget/widget.h"

namespace azer {
namespace views {

RootView::RootView(const gfx::Rect& bounds) {
  host_.reset(widget::WidgetTreeHost::Create(bounds));
  widget_.reset(host_->root());
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

widget::WidgetTreeHost* RootView::GetWidgetTreeHost() { 
  DCHECK(host_.get());
  return host_.get();
}
}  // namespace views
}  // namespace azer
