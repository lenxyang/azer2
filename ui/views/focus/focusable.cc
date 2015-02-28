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

const Focusable* Focusable::GetNext() const {
  Focusable* cur = next_sibling_;
  while (cur != this) {
    if (!cur) {cur = group_->GetHead();}
    if (cur->fosuable()) {
      return cur;
    }

    cur = cur->next_sibling_;
  }

  return this;
}
}  // namespace views
