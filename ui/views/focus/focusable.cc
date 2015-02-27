#include "azer/ui/views/focus/focusable.h"

namespace views {
Focusable::Focusable() 
    : group_head_(NULL)
    , prev_sibling_(NULL)
    , next_sibling_(NULL)
    , group_id_(-1) {
}

Focusable::~Focusable() {
}
}  // namespace views
