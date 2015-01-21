#include "azer/ui/widget/client/screen_position_client.h"

#include "azer/ui/widget/widget_property.h"
#include "azer/ui/widget/widget.h"

DECLARE_WINDOW_PROPERTY_TYPE(azer::widget::client::ScreenPositionClient*)

namespace azer {
namespace widget {
namespace client {
DEFINE_LOCAL_WINDOW_PROPERTY_KEY(ScreenPositionClient*,
                                 kScreenPositionClientKey,
                                 NULL);

void SetScreenPositionClient(Widget* root_window,
                             ScreenPositionClient* client) {
  DCHECK_EQ(root_window->GetRootWidget(), root_window);
  root_window->SetProperty(kScreenPositionClientKey, client);
}

ScreenPositionClient* GetScreenPositionClient(const Widget* root_window) {
  if (root_window)
    DCHECK_EQ(root_window->GetRootWidget(), root_window);
  return root_window ?
      root_window->GetProperty(kScreenPositionClientKey) : NULL;
}
}  // namespace client
}  // namespace widget
}  // namespace azer
