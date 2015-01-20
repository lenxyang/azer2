#pragma once

#include <map>
#include "azer/base/export.h"

namespace azer {
namespace views {

class View;

class FocusGroup {
 public:
  FocusGroup();
  ~FocusGroup();

  void AddView(View* view);
  void RemoveView(View* view);

  View* GetNextView(View*);
 private:
  std::vector<View*> vec_;
  DISALLOW_COPY_AND_ASSIGN(FocusGroup);
};

class FocusManager {
 public:
  FocusManager();
  ~FocusManager();

  FocusGroup* GetGroup(int id);
  FocusGroup* GetNextGroup(int id);
 private:
  typedef std::map<int, FocusGroup*> group_;
  DISALLOW_COPY_AND_ASSIGN(FocusManager);
};
}  // namespace views
}  // namespace azer
