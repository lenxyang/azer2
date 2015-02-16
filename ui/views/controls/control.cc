#include "azer/ui/views/controls/control.h"

namespace views {
Control::Control()
    : group_id_(-1)
    , enabled_(true) {
}

Control::~Control() {
}

}  // namespace views
