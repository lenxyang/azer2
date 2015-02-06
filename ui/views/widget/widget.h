#pragma once

#include "azer/ui/views/views_export.h"
#include "azer/ui/aura/window.h"

namespace views {

class FocusManager;
class FocusTraversable;
class RootView;
class View;
class InputMethod;

class VIEWS_EXPORT Widget {
 public:
  bool IsVisible();
  void UpdateRootLayers();
  void NotifyWillRemoveView(View* view);

  RootView* GetRootView();
  const RootView* GetRootView();
  FocusManager* GetMocusManager();
  bool IsMouseEventsEnabled();
  InputMethod* GetInputMethod();
  const InputMethod* GetInputMethod();
 protected:
  DISALLOW_COPY_AND_ASSIGN(Widget);
};
}  // namespace views
