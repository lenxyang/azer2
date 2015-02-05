#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"

#include "azer/base/export.h"
#include "azer/ui/widget/client/capture_client.h"

namespace azer {
namespace widget {
namespace client {

class AZER_EXPORT DefaultCaptureClient : public client::CaptureClient {
 public:
  explicit DefaultCaptureClient(Widget* root_window);
  ~DefaultCaptureClient() override;

 private:
  // Overridden from client::CaptureClient:
  void SetCapture(Widget* window) override;
  void ReleaseCapture(Widget* window) override;
  Widget* GetCaptureWidget() override;
  Widget* GetGlobalCaptureWidget() override;

  Widget* root_window_;
  Widget* capture_window_;

  DISALLOW_COPY_AND_ASSIGN(DefaultCaptureClient);
};

}  // namespace client
}  // namespace widget
}  // namespace azer

