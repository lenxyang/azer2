#pragma once

#include "azer/ui/views/views_export.h"
#include "azer/ui/aura/window.h"

namespace views {

class FocusManager;
class FocusTraversable;
class RootView;
class View;
class InputMethod;


class VIEWS_EXPORT WindowOwner {
 public:
  WindowOwner();
  ~WindowOwner();
  ui::Layer* layer();
 private:
  DISALLOW_COPY_AND_ASSIGN(WindowOwner);
};


class Widget {
 public:
  bool IsVisible();
  void UpdateRootLayers();
  void NotifyWillRemoveView(View* view);

  RootView* GetRootView() { return NULL;}
  const RootView* GetRootView() const { return NULL;}
  FocusManager* GetMocusManager() { return NULL;}
  bool IsMouseEventsEnabled() { return true;}
  InputMethod* GetInputMethod() { return NULL;}
  const InputMethod* GetInputMethod() const { return NULL;}
 private:
};
}  // namespace views
