#pragma once

#include "base/memory/ref_counted.h"
#include "ui/gfx/native_widget_types.h"
#include "azer/render/swap_chain.h"
#include "azer/render/renderer.h"
#include "azer/render/overlay.h"
#include "azer/render/surface.h"

namespace views {
class Widget;
}

namespace azer {
class RenderSystem;

class AZER_EXPORT WidgetRendererContext {
 public:
  WidgetRendererContext(views::Widget* widget);
  virtual ~WidgetRendererContext();

  views::Widget* widget() { return widget_;}

  SwapChainPtr& GetSwapChain() { return swapchain_;}
  RendererPtr& GetRenderer() { return renderer_;}
  SurfacePtr& GetSurface() { return surface_;}

  void RenderUI();
  void Present();
 protected:
  // The root of the Layer tree drawn by this compositor.
  SurfacePtr surface_;
  SwapChainPtr swapchain_;
  RendererPtr renderer_;
  OverlayPtr overlay_;
  
  RenderSystem* render_system_;
  views::Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(WidgetRendererContext);
};

}  // namespace aze
