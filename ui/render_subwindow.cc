#include "azer/ui/render_subwindow.h"

#include "ui/views/widget/desktop_aura/desktop_screen.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/root_view.h"
#include "azer/ui/adapter/util.h"
#include "azer/ui/adapter/swapchain_context.h"
#include "azer/ui/render_delegate.h"
#include "azer/ui/render_loop.h"
#include "azer/ui/render_view.h"
#include "azer/ui/window_context.h"

namespace azer {

namespace {
class EventView : public views::internal::RootView {
 public:
  static const char kViewClassName[];
  EventView(views::Widget* widget, RenderSubWindow* window);

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
  RenderSubWindow* window_;
  DISALLOW_COPY_AND_ASSIGN(EventView);
};  


// About KeyEvent
// when keyevent pressed, the width(not top level, won't get key down event)
// so we transfer the focus to RenderView it will get the message when focused.
// all mouse event the RenderWidget can get, so it should put all the message
// to RenderView
class RenderSubWindowWidget : public views::Widget {
 public:
  RenderSubWindowWidget(RenderSubWindow* window);
  views::internal::RootView* CreateRootView() override;
  void OnNativeFocus() override;
 private:
  RenderSubWindow* window_;
  DISALLOW_COPY_AND_ASSIGN(RenderSubWindowWidget);
};
}  // namespace

const char RenderSubWindow::kViewClassName[] = "azer::RenderSubWindow";
RenderSubWindow::RenderSubWindow(RenderView* view, RenderDelegate* delegate)
    : Window(Window::GetWindow(view)),
      render_ui_(false),
      need_render_(true),
      realtime_render_(true),
      message_loop_(NULL),
      render_view_(view),
      delegate_(delegate) {
}

RenderSubWindow::~RenderSubWindow() {}

const char* RenderSubWindow::GetClassName() const {
  return kViewClassName;
}

void RenderSubWindow::OnFocus() {
  LOG(ERROR) << "Onfocus: " << GetClassName();
  views::View::OnFocus();
}

views::NonClientFrameView* RenderSubWindow::CreateNonClientFrameView(
    views::Widget* widget) {
  return NULL;
}

views::Widget* RenderSubWindow::CreateWidget() {
  return new RenderSubWindowWidget(this);
}

void RenderSubWindow::OnBeforeWidgetInit(views::Widget::InitParams* params,
                                      views::Widget* widget) {
  params->type = views::Widget::InitParams::TYPE_CONTROL;
  params->child = true;
  params->activatable = views::Widget::InitParams::ACTIVATABLE_NO;
  params->opacity = views::Widget::InitParams::INFER_OPACITY;
  params->bounds  = gfx::Rect(0, 0, 800, 600);
  params->shadow_type = views::Widget::InitParams::SHADOW_TYPE_NONE;
  params->native_widget = context()->CreateDesktopWidget(widget);
  params->parent = GetView()->GetWidget()->GetNativeView();
}

void RenderSubWindow::SetRenderDelegate(RenderDelegate* delegate) {
  DCHECK(!delegate_);
  delegate_ = delegate;
}

void RenderSubWindow::OnAfterWidgetInit() {
  DCHECK(delegate_) << "Render Delegate not specified.";  
  azer::SetRendererWindow(GetWidget());
  GetWidget()->AddObserver(this);
  GetWidget()->Show();
}


// render
bool RenderSubWindow::Init(RenderLoop* loop) {
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

void RenderSubWindow::FirstRender() {
  delegate_->OnUpdate(message_loop_->frame_args());
}

SwapChainPtr& RenderSubWindow::GetSwapChain() {
  return render_context_->GetSwapChain();
}

RendererPtr& RenderSubWindow::GetRenderer() {
  return render_context_->GetRenderer();
}

void RenderSubWindow::Render(const FrameArgs& frame_data) {
  if (need_render_ || realtime_render_) {
    delegate_->OnRender(frame_data);
    if (render_ui_) {
      render_context_->RenderUI();
    }
    delegate_->OnUpdate(frame_data);
    need_render_ = false;
  }
}

void RenderSubWindow::SetRealTimeRender(bool v) {
  realtime_render_ = v;
}

void RenderSubWindow::SetNeedRender() {
  need_render_ = true;
}

void RenderSubWindow::Present() {
  render_context_->Present();
}

// override from RenderSubWindow
void RenderSubWindow::OnWidgetDestroying(views::Widget* widget) {
  widget->RemoveObserver(this);
}

void RenderSubWindow::OnWidgetDestroyed(views::Widget* widget) {
  render_context_->Clear();
}

bool RenderSubWindow::OnMousePressed(const ui::MouseEvent& event) {
  return views::View::OnMousePressed(event);
}

bool RenderSubWindow::OnMouseDragged(const ui::MouseEvent& event) {
  return views::View::OnMouseDragged(event);
}

void RenderSubWindow::OnMouseReleased(const ui::MouseEvent& event) {
  views::View::OnMouseReleased(event);
}

void RenderSubWindow::OnMouseMoved(const ui::MouseEvent& event) {
  views::View::OnMouseMoved(event);
}

bool RenderSubWindow::OnKeyPressed(const ui::KeyEvent& event) {
  return views::View::OnKeyPressed(event);
}

bool RenderSubWindow::OnKeyReleased(const ui::KeyEvent& event)  {
  return views::View::OnKeyReleased(event);
}

bool RenderSubWindow::OnMouseWheel(const ui::MouseWheelEvent& event)  {
  return views::View::OnMouseWheel(event);
}

namespace {
const char EventView::kViewClassName[] = "nelf::EventView::EventView";
EventView::EventView(views::Widget* widget, RenderSubWindow* window)
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

RenderSubWindowWidget::RenderSubWindowWidget(RenderSubWindow* window) 
    : window_(window) {
}

views::internal::RootView* RenderSubWindowWidget::CreateRootView() {
  return new EventView(this, window_);
}

void RenderSubWindowWidget::OnNativeFocus() {
  window_->GetView()->RequestFocus();
  views::Widget::OnNativeFocus();
}
}  // namespace
}  // namespace nelf
