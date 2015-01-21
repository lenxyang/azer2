#include "azer/ui/views/focus_manager.h"

namespace azer {
namespace views {
FocusGroup::FocusGroup(int id)
    : id_(id)
    , view_(NULL) {
}

FocusGroup::~FocusGroup() {
}

void FocusGroup::AddView(View* view) {
}

void FocusGroup::RemoveView(View* view) {
}

View* FocusGroup::GetNextView(View*) {
}

bool FocusGroup::focusable() {
  return true;
}

FocusManager::FocusManager() 
    : focus_group_(-1) {
}

FocusManager::~FocusManager() {
}

FocusGroup* FocusManager::NextGroup() {
  if (group_.empty()) { return NULL;}
  FocusGroup* group = GetNextGroup(current());
  if (group) {
    focus_group_ = group->id();
  }
}

FocusGroup* FocusManager::GetGroup(int id) {
  auto iter = group_.find(id);
  if (group_.end() != iter) {
    return iter->second;
  } else {
    return NULL;
  }
}

FocusGroup* FocusManager::GetNextGroup(int id) {
  auto iter = group_.lower_bound(id);
  if (iter == group_.end()) {
    return group_.begin().second;
  }

  iter++;
  if (iter == group_.end()) {
    iter = group_.begin();
  }

  FocusGroup* group = iter->second;
  if (group->focusable()) {
    return group;
  } else {
    return GetNextGroup(group->id() + 1);
  }
}
}  // namespace views
}  // namespace azer
