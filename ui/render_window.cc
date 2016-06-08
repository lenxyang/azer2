#include "azer/ui/render_window.h"

#include "ui/views/widget/widget.h"
#include "azer/ui/adapter/util.h"
#include "azer/ui/adapter/swapchain_context.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"
#include "azer/ui/render_delegate.h"
#include "azer/ui/render_loop.h"
#include "azer/ui/render_view.h"
#include "azer/ui/render_widget.h"

namespace azer {

const char RenderWindow::kViewClassName[] = "azer::RenderWindow";
RenderWindow::RenderWindow(RenderView* view, RenderDelegate* delegate)
    : Window(Window::GetWindow(view)),
      render_ui_(false),
      need_render_(true),
      realtime_render_(true),
      message_loop_(NULL),
      render_view_(view),
      delegate_(NULL) {
}

RenderWindow::~RenderWindow() {}

const char* RenderWindow::GetClassName() const {
  return kViewClassName;
}

void RenderWindow::OnFocus() {
  LOG(ERROR) << "Onfocus: " << GetClassName();
  views::View::OnFocus();
}

views::NonClientFrameView* RenderWindow::CreateNonClientFrameView(
    views::Widget* widget) {
  return NULL;
}

views::Widget* RenderWindow::CreateWidget() {
  return new RenderWindowWidget(this);
}

void RenderWindow::OnBeforeWidgetInit(views::Widget::InitParams* params,
                                      views::Widget* widget) {
  params->type = views::Widget::InitParams::TYPE_CONTROL;
  params->activatable = views::Widget::InitParams::ACTIVATABLE_NO;
  params->opacity = views::Widget::InitParams::INFER_OPACITY;
  params->bounds  = gfx::Rect(0, 0, 800, 600);
  params->shadow_type = views::Widget::InitParams::SHADOW_TYPE_NONE;
  params->child = true;
  params->native_widget = views::DesktopNativeWidgetAura(widget);
}

void RenderWindow::SetRenderDelegate(RenderDelegate* delegate) {
  DCHECK(!delegate_);
  delegate_ = delegate;
}

void RenderWindow::OnAfterWidgetInit() {
  DCHECK(delegate_) << "Render Delegate not specified.";  
  azer::SetRendererWindow(GetWidget());
  GetWidget()->AddObserver(this);
  GetWidget()->Show();
}


// render
bool RenderWindow::Init(RenderLoop* loop) {
  DCHECK(delegate_);
  Window::Init();
  DCHECK(!render_context_);
  render_context_.reset(new azer::SwapchainContext(GetWidget()));
  render_context_->SetExpectedSampleDesc(GetView()->sample_desc());
  render_context_->Reset();

  message_loop_ = loop;
  delegate_->window_ = this;
  delegate_->view_ = render_view_;
  if (!delegate_->Initialize()) {
    return false;
  }

  return true;
}

void RenderWindow::FirstRender() {
  delegate_->OnUpdate(message_loop_->frame_args());
}

SwapChainPtr& RenderWindow::GetSwapChain() {
  return render_context_->GetSwapChain();
}

RendererPtr& RenderWindow::GetRenderer() {
  return render_context_->GetRenderer();
}

void RenderWindow::Render(const FrameArgs& frame_data) {
  if (need_render_ || realtime_render_) {
    delegate_->OnRender(frame_data);
    if (render_ui_) {
      render_context_->RenderUI();
    }
    delegate_->OnUpdate(frame_data);
    need_render_ = false;
  }
}

void RenderWindow::SetRealTimeRender(bool v) {
  realtime_render_ = v;
}

void RenderWindow::SetNeedRender() {
  need_render_ = true;
}

void RenderWindow::Present() {
  render_context_->Present();
}

// override from RenderWindow
void RenderWindow::OnWidgetDestroying(views::Widget* widget) {
  widget->RemoveObserver(this);
}

void RenderWindow::OnWidgetDestroyed(views::Widget* widget) {
  render_context_->Clear();
}

bool RenderWindow::OnMousePressed(const ui::MouseEvent& event) {
  return views::View::OnMousePressed(event);
}

bool RenderWindow::OnMouseDragged(const ui::MouseEvent& event) {
  return views::View::OnMouseDragged(event);
}

void RenderWindow::OnMouseReleased(const ui::MouseEvent& event) {
  views::View::OnMouseReleased(event);
}

void RenderWindow::OnMouseMoved(const ui::MouseEvent& event) {
  views::View::OnMouseMoved(event);
}

bool RenderWindow::OnKeyPressed(const ui::KeyEvent& event) {
  return views::View::OnKeyPressed(event);
}

bool RenderWindow::OnKeyReleased(const ui::KeyEvent& event)  {
  return views::View::OnKeyReleased(event);
}

bool RenderWindow::OnMouseWheel(const ui::MouseWheelEvent& event)  {
  return views::View::OnMouseWheel(event);
}
}  // namespace nelf
