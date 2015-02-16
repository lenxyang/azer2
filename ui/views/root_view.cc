#include "azer/ui/views/root_view.h"

#include "azer/ui/aura/window_tree_host.h"
#include "azer/ui/aura/window.h"

#include "azer/ui/views/aura/focus_client.h"

namespace views {

RootView::RootView() 
    : closing_(false) {
}

RootView::~RootView() {
  window_.reset();
  host_->RemoveObserver(this);
  focus_client_.reset();
  host_.reset();
}

void RootView::Init(const InitParams& params) {
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

void RootView::Close() {
  closing_ = true;
}

void RootView::Show() {
  host_->Show();
}

void RootView::Hide() {
  host_->Hide();
}
}  // namespace views
