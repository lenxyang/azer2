#pragma once

#include "azer/base/export.h"

namespace azer {
namespace widget {

class Widget;
namespace client {

// This interface provides API to change the root Window's capture state without
// exposing them as RootWindow API.

class AZER_EXPORT CaptureDelegate {
 public:
  // Called when a capture is set on |new_capture| and/or a capture is
  // released on |old_capture|.
  // NOTE: |old_capture| and |new_capture| are not necessarily contained in the
  // window hierarchy of the delegate.
  virtual void UpdateCapture(Widget* old_capture, Widget* new_capture) = 0;

  // Called when another root gets capture.
  virtual void OnOtherRootGotCapture() = 0;

  // Sets/Release a native capture on host windows.
  virtual void SetNativeCapture() = 0;
  virtual void ReleaseNativeCapture() = 0;

 protected:
  virtual ~CaptureDelegate() {}
};

}  // namespace client
}  // namespace widget
}  // namespace azer

