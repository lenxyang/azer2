#include "azer/ui/window.h"

namespace azer {
Window::Window(gfx::Rect bounds)
    : init_bounds_(bounds) {
  InitValue();
}

void Window::InitValue() {
  show_title_ = false;
  show_icon_ = false;
  can_resize_ = false;
  can_maximize_ = false;
  can_minimize_ = false;
}

Window::~Window() {}

void Window::SetTitle(const ::base::string16& title) {
  title_ = title;
}

void Window::SetCanResize(bool value) { 
  can_resize_ = value;
}

void Window::SetCanMinimize(bool value) { 
  can_minimize_ = value;
}

void Window::SetCanMaximize(bool value) { 
  can_maximize_ = value;
}

void Window::SetShowIcon(bool value) {
  show_icon_ = value;
}

void Window::SetShowTitle(bool value) {
  show_title_ = value;
}

void Window::SetMinSize(const gfx::Size& size) {
  minsize_ = size;
}

void Window::SetMaxSize(const gfx::Size& size) {
  maxsize_ = size;
}

// override from views::View 
gfx::Size Window::GetMinimumSize() const {
  if (minsize_.IsEmpty()) {
    return context_->setting().pane_minsize();
  } else {
    return minsize_;
  }
}

gfx::Size Window::GetMaximumSize() const {
  return maxsize_;
}

bool Window::ShouldShowWindowIcon() const {
  return show_icon_;
}

bool Window::ShouldShowWindowTitle() const {
  return show_title_;
}

bool Window::CanResize() const { 
  return can_resize_; 
}

bool Window::CanMaximize() const { 
  return can_maximize_; 
}

bool Window::CanMinimize() const { 
  return can_minimize_; 
}

base::string16 Window::GetWindowTitle() const {
  return title_;
}

views::View* Window::GetContentsView() { 
  return this; 
}

void Window::Show() {
  GetWidget()->Show();
}

void Window::ShowWithCapture() {
  Show();
  GetWidget()->native_widget_private()->SetCapture();
  GetWidget()->SetCapture(this);
}

void Window::Init() {
  DCHECK(!GetWidget());
  using views::Widget;
  Widget* widget = CreateWidget();
  Widget::InitParams wparams;
  wparams.parent = GetParentNativeView();
  wparams.type = Widget::InitParams::TYPE_WINDOW;
  wparams.delegate = this;
  wparams.context  = NULL;
  wparams.show_state = ui::SHOW_STATE_DEFAULT;
  OnBeforeWidgetInit(&wparams, widget);
  if (!wparams.native_widget) {
    wparams.native_widget = context_->adapter()->CreateDesktopWidget(widget);
  }
  
  if (!init_bounds_.IsEmpty()) {
    wparams.bounds = init_bounds_;
  } else if (wparams.bounds.IsEmpty()) {
    wparams.bounds = gfx::Rect(context_->setting().pane_default_size());
  }
  widget->Init(wparams);
  if (mainframe_) {
    SetMainWidgetBelonged(widget, mainframe_->GetWidget());
  }

  OnAfterWidgetInit();
}

views::Widget* Window::CreateWidget() {
  return new BaseWidget;
}

void Window::OnBeforeWidgetInit(views::Widget::InitParams* params,
                                views::Widget* widget) {
  params->type = views::Widget::InitParams::TYPE_WINDOW;
  params->opacity = views::Widget::InitParams::INFER_OPACITY;
  params->keep_on_top = false;
}

void Window::OnAfterWidgetInit() {
}

const char* Window::GetClassName() const {
  return kViewClassName;
}

void Window::OnFocus() {
  views::View::OnFocus();
}

void Window::SetWindowIcon(gfx::ImageSkia icon) {
  window_icon_ = icon;
}

void Window::SetAppIcon(gfx::ImageSkia icon) {
  app_icon_ = icon;
}

}  // namespace azer
