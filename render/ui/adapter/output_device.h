#pragma once

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "cc/output/software_output_device.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/vector2d.h"

#include "azer/base/export.h"
#include "azer/base/back_swap.h"
#include "azer/render/context2d.h"
#include "azer/render/canvas2d.h"

class SkBitmap;
class SkCanvas;

namespace gfx {
class VSyncProvider;
}

namespace cc {
class SoftwareFrameData;
}  // namespace cc

namespace views {
class Widget;
}  // namespace views

namespace azer {
// This is a "tear-off" class providing software drawing support to
// OutputSurface, such as to a platform-provided window framebuffer.
class AZER_EXPORT TextureOutputDevice : public cc::SoftwareOutputDevice {
 public:
  TextureOutputDevice(azer::RenderSystem* render_system,
                      views::Widget* widget);
  ~TextureOutputDevice() override;

  // Discards any pre-existing backing buffers and allocates memory for a
  // software device of |size|. This must be called before the
  // |TextureOutputDevice| can be used in other ways.
  void Resize(const gfx::Size& pixel_size, float scale_factor) override;

  // Called on BeginDrawingFrame. The compositor will draw into the returned
  // SkCanvas. The |TextureOutputDevice| implementation needs to provide a
  // valid SkCanvas of at least size |damage_rect|. This class retains ownership
  // of the SkCanvas.
  SkCanvas* BeginPaint(const gfx::Rect& damage_rect) override;

  // Called on FinishDrawingFrame. The compositor will no longer mutate the the
  // SkCanvas instance returned by |BeginPaint| and should discard any reference
  // that it holds to it.
  void EndPaint(cc::SoftwareFrameData* frame_data) override;

  // Copies pixels inside |rect| from the current software framebuffer to
  // |pixels|. Fails if there is no current softwareframebuffer.
  void CopyToPixels(const gfx::Rect& rect, void* pixels) override;

  // Blit the pixel content of the SwapchainOutputDevice by |delta| with the
  // write clipped to |clip_rect|.
  void Scroll(const gfx::Vector2d& delta, const gfx::Rect& clip_rect) override;

  // Discard the backing buffer in the surface provided by this instance.
  void DiscardBackbuffer() override {}

  // Ensures that there is a backing buffer available on this instance.
  void EnsureBackbuffer() override {}

  // TODO(skaslev) Remove this after UberCompositor lands.
  // Called in response to receiving a SwapBuffersAck. At this point, software
  // frame identified by id can be reused or discarded as it is no longer being
  // displayed.
  void ReclaimSoftwareFrame(unsigned id) override;

  // VSyncProvider used to update the timer used to schedule draws with the
  // hardware vsync. Return NULL if a provider doesn't exist.
  gfx::VSyncProvider* GetVSyncProvider() override;

  azer::Canvas2DPtr GetCanvas() { return canvas2d_;}

  void SetResizeCallback(::base::Callback<void(void)> closure);
 protected:
  gfx::Size viewport_pixel_size_;
  float scale_factor_;
  gfx::Rect damage_rect_;
  scoped_ptr<gfx::VSyncProvider> vsync_provider_;
  azer::Canvas2DPtr canvas2d_;
  azer::Context2D* context2d_;
  azer::RenderSystem* render_system_;
  views::Widget* widget_;
  ::base::Callback<void(void)> resize_closure_;
 private:
  DISALLOW_COPY_AND_ASSIGN(TextureOutputDevice);
};

}  // namespace azer
