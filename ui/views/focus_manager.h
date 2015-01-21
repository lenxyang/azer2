#pragma once

#include <map>
#include "azer/base/export.h"
#include "azer/ui/views/view_observer.h"

namespace azer {
namespace views {

class View;

class FocusGroup : public ViewObserver {
 public:
  explicit FocusGroup(int id);
  ~FocusGroup();

  void AddView(View* view);
  void RemoveView(View* view);

  View* GetCurrent();
  View* GetNextView();
  // is there any one of view is focusable
  // if view is disabled, hideen or not focusable, 
  bool focusable();
  int id() const { return id_;}
 private:
  int id_;
  View* current_;
  std::vector<View*> vec_;
  DISALLOW_COPY_AND_ASSIGN(FocusGroup);
};

class FocusManager {
 public:
  FocusManager();
  ~FocusManager();

  FocusGroup* GetGroup(int id);
  FocusGroup* NextGroup();

  int current() { return focus_group_;}
 private:
  FocusGroup* GetNextGroup(int id);
  int focus_group_;
  typedef std::map<int, FocusGroup*> group_;
  DISALLOW_COPY_AND_ASSIGN(FocusManager);
};
}  // namespace views
}  // namespace azer
