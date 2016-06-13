#pragma once

#include "base/memory/ref_counted.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/widget/widget_observer.h"

#include "azer/render/swap_chain.h"
#include "azer/render/renderer.h"
#include "azer/effect/overlay.h"
#include "azer/render/surface.h"

namespace azer {

class SwapchainContext : public views::WidgetObserver {
 public:
  SwapchainContext(views::Widget* widget);
  virtual ~SwapchainContext();

  views::Widget* widget() { return widget_;}

  void SetExpectedSampleDesc(const SampleDesc& desc) {
    sample_desc_ = desc;
  }

  SwapChainPtr& GetSwapChain() { return swapchain_;}
  RendererPtr& GetRenderer() { return renderer_;}
  SurfacePtr& GetSurface() { return surface_;}
  void Clear();
  void Reset();

  virtual void RenderUI();
  virtual void Present();
 protected:
  // The root of the Layer tree drawn by this compositor.
  virtual Surface* CreateSurfaceForWidget(views::Widget* widget);
  void ResetSwapchain();

  // override from WidgetObserver
  void OnWidgetBoundsChanged(views::Widget* widget, 
                             const gfx::Rect& new_bounds) override;

  SurfacePtr surface_;
  SwapChainPtr swapchain_;
  RendererPtr renderer_;
  RasterizerStatePtr rasterizer_state_;
  DepthStencilStatePtr depth_state_;
  OverlayPtr overlay_;
  RenderSystem* render_system_;
  SampleDesc sample_desc_;
  views::Widget* widget_;
  int id_;
  DISALLOW_COPY_AND_ASSIGN(SwapchainContext);
};

}  // namespace azer
