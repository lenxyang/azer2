#pragma once

#include "base/basictypes.h"
#include "azer/ui/views/views_export.h"

namespace views {

class VIEWS_EXPORT FocusableDelegate {
 public:
  FocusableDelegate() {}
  virtual void OnFocusableGetFocus() = 0;
  virtual void OnFocusableLostFocus() = 0;
  // when the accelerator pressed
  virtual void OnFocusableAcceleratorPressed() = 0;
};

class VIEWS_EXPORT Focusable {
 public:
  Focusable();
  virtual ~Focusable();

  Focusable* GetNext() {
    return const_cast<Focusable*>(
        static_cast<const Focusable*>(this)->GetNext());
  }
  const Focusable* GetNext() const;

  FocusGroup* GetGroup() { return group_;}

  void SetFocusable();
  bool focusable() const;
 private:
  Focusable* next_sibling_;
  Focusable* prev_sibling_;
  FocusGroup* group_;

  friend class FocusGroup;
  DISALLOW_COPY_AND_ASSIGN(Focusable);
};

}  // namespace views
