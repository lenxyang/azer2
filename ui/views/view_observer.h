#pragma once

#include "azer/base/export.h"

namespace azer {
namespace views {
class AZER_EXPORT ViewObserver {
 public:
  virtual void OnSetFocusable(View* view) {}
  virtual void OnVisiblityChanging(View* view, bool prevval, bool newval) {}
  virtual void OnVisiblityChanged(View* view, bool prevval, bool newval) {}
  virtual void OnEnableStateChanging(View* view, bool prevval, bool newval) {}
  virtual void OnEnableStateChanged(View* view, bool prevval, bool newval) {}
 protected:
  ViewObserver() {}
  virtual ~ViewObserver() {}
  DISALLOW_COPY_AND_ASSIGN(ViewObserver);
};
}  // namespace views
}  // namespace azer
