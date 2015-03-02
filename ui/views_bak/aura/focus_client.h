#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/scoped_observer.h"
#include "azer/ui/aura/client/focus_client.h"
#include "azer/ui/aura/window_observer.h"
#include "azer/ui/views/views_export.h"

namespace views {

class VIEWS_EXPORT FocusClient : public aura::client::FocusClient,
                                 public aura::WindowObserver {
 public:
  FocusClient();
  ~FocusClient() override;

 private:
  // Overridden from client::FocusClient:
  void AddObserver(aura::client::FocusChangeObserver* observer) override;
  void RemoveObserver(aura::client::FocusChangeObserver* observer) override;
  void FocusWindow(aura::Window* window) override;
  void ResetFocusWithinActiveWindow(aura::Window* window) override;
  aura::Window* GetFocusedWindow() override;

  // Overridden from WindowObserver:
  void OnWindowDestroying(aura::Window* window) override;

  aura::Window* focused_window_;
  ScopedObserver<aura::Window, aura::WindowObserver> observer_manager_;

  DISALLOW_COPY_AND_ASSIGN(FocusClient);
};

}  // namespace views
