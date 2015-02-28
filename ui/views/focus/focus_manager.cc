#include "azer/ui/views/focus/focus_manger.h"

namespace views {
FocusManager::FocusManager() 
    : focused_(NULL) {
}

FocusManager::~FocusManager() {
}

void FocusManager::SetFocused(Focusable* focusable) {
}

void FocusManager::ClearFocus() {
  focused_ = NULL;
}
}  // namespace views
