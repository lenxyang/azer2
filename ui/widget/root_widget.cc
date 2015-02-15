#include "azer/ui/widget/root_widget.h"

#include "azer/ui/aura/window_tree_host.h"
#include "azer/ui/aura/window.h"

namespace widget {

RootWidget::RootWidget() 
    : closed_(false) {
}

RootWidget::~RootWidget() {
  window_.reset();
  host_->RemoveObserver(this);
  focus_client_.reset();
  host_.reset();
}

void RootWidget::Init(const InitParams& param) {
  host_.reset(aura::WindowTreeHost::Create(params.bounds));
  host_->window()->SetBounds(gfx::Rect(params.bounds.size()));
  focus_client_.reset(new FocusClient);
  aura::client::SetFocusClient(host_->window(), focus_client_.get());

  window_.reset(new aura::Window(NULL));
  window()->Init(aura::WINDOW_LAYER_NOT_DRAWN);
  window()->set_id(0);
  window()->SetBounds(gfx::Rect(params.bounds.size()));
  window()->SetName("ContentWindowContainer");
  window()->Show();

  host_->InitHost();
  host_->window()->AddChild(window());
  host_->AddObserver(this);
}

void RootWidget::Close() {
  closing_ = true;
}
}  // namespace widget
