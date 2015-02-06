#include "azer/ui/views/widget/widget.h"

namespace views {
bool Widget::IsVisible() {
  return true;
}

void Widget::UpdateRootLayers() {
}

void Widget::NotifyWillRemoveView(View* view) {
}

RootView* Widget::GetRootView() { 
  return NULL;
}

const RootView* Widget::GetRootView() const { 
  return NULL;
}

FocusManager* Widget::GetMocusManager() { 
  return NULL;
}

bool Widget::IsMouseEventsEnabled() { 
  return true;
}

InputMethod* Widget::GetInputMethod() { 
  return NULL;
}

const InputMethod* Widget::GetInputMethod() const { 
  return NULL;
}
}  // namespace views {
