#pragma once

#include "azer/base/export.h"

namespace azer {
namespace widget {
class Widget;

namespace client {
// An interface implemented by an object that manages input capture.
class AZER_EXPORT CaptureClient {
 public:
  // Does a capture on the |window|.
  virtual void SetCapture(Widget* window) = 0;

  // Releases a capture from the |window|.
  virtual void ReleaseCapture(Widget* window) = 0;

  // Returns the current capture window. This may only return a Widget if the
  // Widget that has capture is a child of the Widget the CaptureClient is
  // installed on. GetGlobalCaptureWidget() can be used to locate the Widget
  // that has capture regardless of the Widget the CaptureClient is installed
  // on.
  virtual Widget* GetCaptureWidget() = 0;

  // See description of GetCaptureWidget() for details.
  virtual Widget* GetGlobalCaptureWidget() = 0;

 protected:
  virtual ~CaptureClient() {}
};

// Sets/Gets the capture client on the root Widget.
AZER_EXPORT void SetCaptureClient(Widget* root_window,
                                  CaptureClient* client);
AZER_EXPORT CaptureClient* GetCaptureClient(Widget* root_window);

// A utility function to get the current capture window. Returns NULL
// if the window doesn't have a root window, or there is no capture window.
AZER_EXPORT Widget* GetCaptureWidget(Widget* window);

}  // namespace client
}  // namespace widget
}  // namespace azer
