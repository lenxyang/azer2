#pragma once

#include "base/memory/ref_counted.h"
#include "ui/gfx/native_widget_types.h"
#include "azer/render/swap_chain.h"
#include "azer/render/renderer.h"
#include "azer/render/overlay.h"
#include "azer/render/surface.h"

namespace azer {

class RenderSystem;

class AZER_EXPORT AppContext {
 public:
  virtual ~AppContext();

  static AppContext* CreateInstance(gfx::AcceleratedWidget widget);
  static AppContext* GetInstance();
  static void DestroyInstance();

  SwapChainPtr& GetSwapchain() { return swapchain_;}
  RendererPtr& GetRenderer() { return renderer_;}
  SurfacePtr& GetSurface() { return surface_;}

  void RenderUI(TexturePtr& texture);
 protected:
  AppContext(gfx::AcceleratedWidget widget);

  // The root of the Layer tree drawn by this compositor.
  SurfacePtr surface_;
  SwapChainPtr swapchain_;
  RendererPtr renderer_;
  OverlayPtr overlay_;
  RenderSystem* render_system_;

  static AppContext* context_;
  DISALLOW_COPY_AND_ASSIGN(AppContext);
};
}  // namespace azer
