#pragma once

#include "base/memory/ref_counted.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/widget/widget_observer.h"
#include "azer/render/swap_chain.h"
#include "azer/render/renderer.h"
#include "azer/render/overlay.h"
#include "azer/render/surface.h"

namespace views {
class Widget;
}

namespace azer {
class RenderSystem;
class RenderLoop;

class AZER_EXPORT WidgetRendererContext : public views::WidgetObserver {
 public:
  WidgetRendererContext(views::Widget* widget, RenderLoop* render_loop);
  virtual ~WidgetRendererContext();

  views::Widget* widget() { return widget_;}

  SwapChainPtr& GetSwapChain() { return swapchain_;}
  RendererPtr& GetRenderer() { return renderer_;}
  SurfacePtr& GetSurface() { return surface_;}

  void RenderUI();
  void Present();
 protected:
  // override from WidgetObserver
  void OnWidgetClosing(views::Widget* widget) override;
  void OnWidgetCreated(views::Widget* widget) override;
  void OnWidgetDestroying(views::Widget* widget) override;
  void OnWidgetDestroyed(views::Widget* widget) override;
  void OnWidgetBoundsChanged(views::Widget* widget, 
                             const gfx::Rect& new_bounds) override;
  // The root of the Layer tree drawn by this compositor.

  Surface* CreateSurfaceForWidget(views::Widget* widget);
  void ResetSwapchain();

  SurfacePtr surface_;
  SwapChainPtr swapchain_;
  RendererPtr renderer_;
  OverlayPtr overlay_;
  
  RenderSystem* render_system_;
  RenderLoop* render_loop_;
  views::Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(WidgetRendererContext);
};

}  // namespace aze
