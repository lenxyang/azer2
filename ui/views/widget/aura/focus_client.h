#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/scoped_observer.h"
#include "azer/ui/aura/client/focus_client.h"
#include "azer/ui/aura/window_observer.h"

namespace views {

class FocusClient : public client::FocusClient,
                    public WindowObserver {
 public:
  TestFocusClient();
  ~TestFocusClient() override;

 private:
  // Overridden from client::FocusClient:
  void AddObserver(client::FocusChangeObserver* observer) override;
  void RemoveObserver(client::FocusChangeObserver* observer) override;
  void FocusWindow(Window* window) override;
  void ResetFocusWithinActiveWindow(Window* window) override;
  Window* GetFocusedWindow() override;

  // Overridden from WindowObserver:
  void OnWindowDestroying(Window* window) override;

  Window* focused_window_;
  ScopedObserver<Window, WindowObserver> observer_manager_;

  DISALLOW_COPY_AND_ASSIGN(TestFocusClient);
};

}  // namespace views
