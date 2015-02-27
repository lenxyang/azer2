#pragma once

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "azer/ui/views/views_export.h"

namespace views {

class Focusable;

class VIEWS_EXPORT FocusManager {
 public:
 private:
  std::map<int64, Focusable> groups_;
  DISALLOW_COPY_AND_ASSIGN(FocusManager);
};
}  // namespace views
