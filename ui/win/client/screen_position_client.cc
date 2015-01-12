#include "azer/ui/win/client/screen_position_client.h"

#include "ui/aura/window_property.h"

namespace azer {
namespace win {

DECLARE_WINDOW_PROPERTY_TYPE(azer::win::ScreenPositionClient*)

DEFINE_LOCAL_WINDOW_PROPERTY_KEY(ScreenPositionClient*,
                                 kScreenPositionClientKey,
                                 NULL);

void SetScreenPositionClient(Window* root_window,
                             ScreenPositionClient* client) {
  DCHECK_EQ(root_window->GetRootWindow(), root_window);
  root_window->SetProperty(kScreenPositionClientKey, client);
}

ScreenPositionClient* GetScreenPositionClient(const Window* root_window) {
  if (root_window)
    DCHECK_EQ(root_window->GetRootWindow(), root_window);
    return root_window ?
        root_window->GetProperty(kScreenPositionClientKey) : NULL;
}

}  // namespace win
}  // namespace azer
