#include "azer/ui/widget/client/capture_client.h"

#include "azer/ui/widget/widget_event_dispatcher.h"
#include "azer/ui/widget/widget_property.h"
#include "azer/ui/widget/widget.h"

DECLARE_WINDOW_PROPERTY_TYPE(azer::widget::client::CaptureClient*)

namespace azer {
namespace widget {
namespace client {
DEFINE_WINDOW_PROPERTY_KEY(
    CaptureClient*, kRootWidgetCaptureClientKey, NULL);

void SetCaptureClient(Widget* root_window, CaptureClient* client) {
  root_window->SetProperty(kRootWidgetCaptureClientKey, client);
}

CaptureClient* GetCaptureClient(Widget* root_window) {
  return root_window ?
      root_window->GetProperty(kRootWidgetCaptureClientKey) : NULL;
}

Widget* GetCaptureWidget(Widget* window) {
  Widget* root_window = window->GetRootWidget();
  if (!root_window)
    return NULL;
  CaptureClient* capture_client = GetCaptureClient(root_window);
  return capture_client ? capture_client->GetCaptureWidget() : NULL;

}  // namespace client
}  // namespace widget
}  // namespace azer
