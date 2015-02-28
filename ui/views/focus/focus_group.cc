#include "azer/ui/views/focus/focus_group.h"


#include "base/logging.h"
#include "azer/ui/views/focus/focusable.h"

namespace views {
FocusGroup::FocusGroup() 
    : head_(NULL)
    , tail_(NULL)
    , group_id_(-1) {
}

void FocusGroup::Add(Focusable* focusable) {
  if (focusable->GetGroup()) {
    focusable->GetGroup()->Remove(focusable);
  }

  if (!head_) {
    CHECK(!tail_);
    head_ = focusable;
    tail_ = focusable;
  } else {
    tail_->next_sibling_ = focusable;
    focusable->prev_sibling_ = tail_->next_sibling_;
    tail_ = focusable;
  }

}

void FocusGroup::Remove(Focusable* focusable) {
  if (head_ = focusable) {
    head_ = focusable->next_sibling_;
  }
  if (tail_ = focusable) {
    tail_ = focusable->prev_sibling_;
  }

  if (focusable->prev_sibling_) {
    focusable->prev_sibling_->next_sibling_ = focusable->next_sibling_;
  }

  if (focusable->next_sibling_) {
    focusable->next_sibling_->prev_sibling_ = focusable->prev_sibling_;
  }

  
  focusable->group_ = NULL;
  focusable->prev_sibling_ = NULL;
  focusable->next_sibling_ = NULL;
}

}  // namespace views
