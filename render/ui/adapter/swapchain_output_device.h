#pragma once

#include <atomic>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "cc/output/software_output_device.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/vector2d.h"
#include "ui/gfx/native_widget_types.h"

#include "azer/base/export.h"
#include "azer/base/back_swap.h"
#include "azer/render/context2d.h"
#include "azer/render/canvas2d.h"
#include "azer/render/swap_chain.h"
#include "azer/uisbox/adapter/base/output_device.h"

class SkBitmap;
class SkCanvas;

namespace gfx {
class VSyncProvider;
}

namespace cc {
class SoftwareFrameData;
}  // namespace cc

class WidgetRendererContext;

class SwapchainOutputDevice : public TextureOutputDevice {
 public:
  SwapchainOutputDevice(azer::RenderSystem* render_system,
                        views::Widget* widget);
  ~SwapchainOutputDevice() override;

  void Resize(const gfx::Size& pixel_size, float scale_factor) override;
  SkCanvas* BeginPaint(const gfx::Rect& damage_rect) override;
  void EndPaint(cc::SoftwareFrameData* frame_data) override;
 protected:

  class SwapChainDeviceRenderer : public
                          ::base::RefCounted<SwapChainDeviceRenderer> {
   public:
    SwapChainDeviceRenderer(WidgetRendererContext* context) ;
    void PostTask();
    void Stop();
   private:
    void RenderProc();
    void PostProc();

    std::atomic<bool> stop_;
    bool first_post_task_;
    WidgetRendererContext* context_;
    friend class SwapchainOutputDevice;
    DISALLOW_COPY_AND_ASSIGN(SwapChainDeviceRenderer);
  };

  azer::SwapChainPtr swapchain_;
  scoped_refptr<SwapChainDeviceRenderer> render_util_;
  scoped_ptr<WidgetRendererContext> widget_context_;
 private:
  DISALLOW_COPY_AND_ASSIGN(SwapchainOutputDevice);
};

