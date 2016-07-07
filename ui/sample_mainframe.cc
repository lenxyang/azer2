#include "azer/ui/sample_mainframe.h"

#include "ui/views/layout/layout_manager.h"

#include "azer/azer.h"
#include "azer/ui/panel/fps_panel.h"
#include "azer/ui/render_loop.h"
#include "azer/ui/render_subwindow.h"
#include "azer/ui/render_view.h"
#include "azer/ui/window_context.h"

namespace azer {
SampleMainframe::SampleMainframe(const gfx::Rect& init_bounds, WindowContext* ctx)
    : Window(init_bounds, ctx),
      render_view_(NULL) {
  Vector3 camera_pos(0.0f, 1.0f, 5.0f);
  Vector3 lookat(0.0f, 1.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);
}

SampleMainframe::~SampleMainframe() {}

void SampleMainframe::InitRenderView() {
  if (!render_loop_.get()) {
    render_loop_ = new RenderLoop(this);
  }

  render_view_ = new RenderView(render_loop_.get(), this);
  render_view_->SetSampleDesc(sample_desc_);
  AddChildView(render_view_);
  render_loop_->AddRenderView(render_view_);
}

void SampleMainframe::OnAfterWidgetInit() {
  Window::OnAfterWidgetInit();
  InitRenderView();
}

// override from RenderDelegate
bool SampleMainframe::Initialize() {
  fpspanel_ = new FPSPanel;
  fpspanel_->SetBounds(10, 10, 180, 50);
  this->window()->AddChildView(fpspanel_);
  this->window()->SetRenderUI(true);
  window()->GetWidget()->AddObserver(this);
  OnInit();
  return true;
}

void SampleMainframe::OnUpdate(const FrameArgs& args) {
  fpspanel_->Update(args);
  OnUpdateFrame(args);
}

void SampleMainframe::OnRender(const FrameArgs& args) {
  Renderer* renderer = window()->GetRenderer().get();
  OnRenderFrame(args, renderer);
}

// override from views::WidgetObserver
void SampleMainframe::OnWidgetBoundsChanged(views::Widget* widget, 
                                         const gfx::Rect& new_bounds) {
  gfx::Rect rect = view()->GetContentsBounds();
  float aspect = (float)rect.width() / (float)rect.height();
  camera_.set_aspect(aspect);
}

void SampleMainframe::OnWidgetDestroying(views::Widget* widget) {
  widget->RemoveObserver(this);
}

void SampleMainframe::WindowClosing() {
  base::MessageLoopForUI::current()->QuitNow();
}
}  // namespace xut
