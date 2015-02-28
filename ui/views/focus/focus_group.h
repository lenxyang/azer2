#pragma once

#include "base/basictypes.h"
#include "azer/ui/views/views_export.h"

namespace views {

class Focusable;

class VIEWS_EXPORT FocusGroup {
 public:
  FocusGroup();
  void Add(Focusable* focusable);
  void Remove(Focusable* focusable);

  Focusable* GetHead() { return head_;}
  const Focusable* GetHead() const { return head_;}
  Focusable* GetTail() { return tail_;}
  const Focusable* GetTail() const { return tail_;}
 private:
  int32 group_id_;
  Focusable* head_;
  Focusable* tail_;
  DISALLOW_COPY_AND_ASSIGN(FocusGroup);
};
}  // namespace views
