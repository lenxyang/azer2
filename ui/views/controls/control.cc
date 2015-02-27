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

void Control::SetGroup(int32 group) {
}
}  // namespace views
