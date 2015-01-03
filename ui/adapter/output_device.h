#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "cc/output/software_output_device.h"
#include "skia/ext/refptr.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/vector2d.h"
#include "azer/base/export.h"

class SkBitmap;
class SkCanvas;

namespace gfx {
class VSyncProvider;
}

namespace cc {
class SoftwareFrameData;
}  // namespace cc

namespace azer {

// This is a "tear-off" class providing software drawing support to
// OutputSurface, such as to a platform-provided window framebuffer.
class AZER_EXPORT Azer2DDevice : public cc::SoftwareOutputDevice {
 public:
  Azer2DDevice();
  virtual ~Azer2DDevice();

  // Discards any pre-existing backing buffers and allocates memory for a
  // software device of |size|. This must be called before the
  // |Azer2DDevice| can be used in other ways.
  virtual void Resize(const gfx::Size& pixel_size, float scale_factor);

  // Called on BeginDrawingFrame. The compositor will draw into the returned
  // SkCanvas. The |Azer2DDevice| implementation needs to provide a
  // valid SkCanvas of at least size |damage_rect|. This class retains ownership
  // of the SkCanvas.
  virtual SkCanvas* BeginPaint(const gfx::Rect& damage_rect);

  // Called on FinishDrawingFrame. The compositor will no longer mutate the the
  // SkCanvas instance returned by |BeginPaint| and should discard any reference
  // that it holds to it.
  virtual void EndPaint(cc::SoftwareFrameData* frame_data);

  // Copies pixels inside |rect| from the current software framebuffer to
  // |pixels|. Fails if there is no current softwareframebuffer.
  virtual void CopyToPixels(const gfx::Rect& rect, void* pixels);

  // Blit the pixel content of the Azer2DDevice by |delta| with the
  // write clipped to |clip_rect|.
  virtual void Scroll(const gfx::Vector2d& delta, const gfx::Rect& clip_rect);

  // Discard the backing buffer in the surface provided by this instance.
  virtual void DiscardBackbuffer() {}

  // Ensures that there is a backing buffer available on this instance.
  virtual void EnsureBackbuffer() {}

  // TODO(skaslev) Remove this after UberCompositor lands.
  // Called in response to receiving a SwapBuffersAck. At this point, software
  // frame identified by id can be reused or discarded as it is no longer being
  // displayed.
  virtual void ReclaimSoftwareFrame(unsigned id);

  // VSyncProvider used to update the timer used to schedule draws with the
  // hardware vsync. Return NULL if a provider doesn't exist.
  virtual gfx::VSyncProvider* GetVSyncProvider();

 protected:
  gfx::Size viewport_pixel_size_;
  float scale_factor_;
  gfx::Rect damage_rect_;
  skia::RefPtr<SkCanvas> canvas_;
  scoped_ptr<gfx::VSyncProvider> vsync_provider_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Azer2DDevice);
};

}  // namespace azer

