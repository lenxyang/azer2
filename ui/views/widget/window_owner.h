#pragma once

#include "azer/ui/views/views_export.h"
#include "azer/ui/aura/window.h"

namespace views {

class VIEWS_EXPORT WindowOwner {
 public:
  WindowOwner();
  ~WindowOwner();
  ui::Layer* layer();
 private:
  DISALLOW_COPY_AND_ASSIGN(WindowOwner);
};

}  // namespace views
