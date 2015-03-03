#pragma once

#include "base/memory/ref_counted.h"
#include "ui/gfx/native_widget_types.h"
#include "azer/render/swap_chain.h"
#include "azer/render/renderer.h"
#include "azer/render/overlay.h"
#include "azer/render/surface.h"

namespace azer {
class RenderSystem;
}  // namespace aze

namespace views {
class Widget;
}

class WidgetRendererContext {
 public:
  WidgetRendererContext(views::Widget* widget);
  virtual ~WidgetRendererContext();

  views::Widget* widget() { return widget_;}

  azer::SwapChainPtr& GetSwapChain() { return swapchain_;}
  azer::RendererPtr& GetRenderer() { return renderer_;}
  azer::SurfacePtr& GetSurface() { return surface_;}

  void RenderUI();
  void Present();
 protected:
  // The root of the Layer tree drawn by this compositor.
  azer::SurfacePtr surface_;
  azer::SwapChainPtr swapchain_;
  azer::RendererPtr renderer_;
  azer::OverlayPtr overlay_;
  
  azer::RenderSystem* render_system_;
  views::Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(WidgetRendererContext);
};
