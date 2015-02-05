#include "azer/ui/widget/client/default_capture_client.h"


#include "azer/ui/widget/client/capture_delegate.h"
#include "azer/ui/widget/widget.h"
#include "azer/ui/widget/widget_event_dispatcher.h"
#include "azer/ui/widget/widget_tree_host.h"

namespace azer {
namespace widget {
namespace client {

DefaultCaptureClient::DefaultCaptureClient(Widget* root_window)
    : root_window_(root_window),
      capture_window_(NULL) {
  SetCaptureClient(root_window_, this);
}

DefaultCaptureClient::~DefaultCaptureClient() {
  SetCaptureClient(root_window_, NULL);
}

void DefaultCaptureClient::SetCapture(Widget* window) {
  if (capture_window_ == window)
    return;

  Widget* old_capture_window = capture_window_;
  capture_window_ = window;

  CaptureDelegate* capture_delegate = root_window_->GetHost()->dispatcher();
  if (capture_window_)
    capture_delegate->SetNativeCapture();
  else
    capture_delegate->ReleaseNativeCapture();

  capture_delegate->UpdateCapture(old_capture_window, capture_window_);
}

void DefaultCaptureClient::ReleaseCapture(Widget* window) {
  if (capture_window_ != window)
    return;
  SetCapture(NULL);
}

Widget* DefaultCaptureClient::GetCaptureWidget() {
  return capture_window_;
}

Widget* DefaultCaptureClient::GetGlobalCaptureWidget() {
  return capture_window_;
}

}  // namespace client
}  // namespace widget
}  // namespace azer
