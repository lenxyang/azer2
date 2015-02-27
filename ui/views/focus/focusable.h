#pragma once

#include "base/basictypes.h"
#include "azer/ui/views/views_export.h"

namespace views {
class VIEWS_EXPORT Focusable {
 public:
  Focusable();
  virtual ~Focusable();

  Focusable* GetNext();
  const Focusable* GetNext() const;

  void Add(Focusable* focusable);
  void Remove(Focusable* focusable);
  bool IsGroupHead() const { return group_head_;}
 private:
  Focusable* next_sibling_;
  Focusable* prev_sibling_;
  int32 group_id_;
  DISALLOW_COPY_AND_ASSIGN(Focusable);
};
}  // namespace views
