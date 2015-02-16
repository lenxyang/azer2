#include "azer/ui/views/controls/control.h"

namespace views {
Control::Control()
    : group_id_(-1) {
}

Control::~Control() {
}

View* Control::GetSelectedViewForGroup(int32 group) {
  return NULL;
}
}  // namespace views
