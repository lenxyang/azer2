#pragma once

#include "azer/base/export.h"

namespace azer {
namespace views {
class AZER_EXPORT ViewObserver {
 public:
  virtual void OnSetFocusable(View* view) {}
  virtual void OnShow(View* view) {}
  virtual void OnHide(View* view) {}
 protected:
  ViewObserver() {}
  virtual ~ViewObserver() {}
  DISALLOW_COPY_AND_ASSIGN(ViewObserver);
};
}  // namespace views
}  // namespace azer
