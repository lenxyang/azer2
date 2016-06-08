#pragma once

#include "base/callback.h"
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

class SwapchainContext;

// This is a "tear-off" class providing software drawing support to
// OutputSurface, such as to a platform-provided window framebuffer.
class OutputDevice : public cc::SoftwareOutputDevice {
 public:
  OutputDevice();
  ~OutputDevice() override;

  // Discards any pre-existing backing buffers and allocates memory for a
  // software device of |size|. This must be called before the
  // |OutputDevice| can be used in other ways.
  void Resize(const gfx::Size& pixel_size, float scale_factor) override;

  // Called on BeginDrawingFrame. The compositor will draw into the returned
  // SkCanvas. The |OutputDevice| implementation needs to provide a
  // valid SkCanvas of at least size |damage_rect|. This class retains ownership
  // of the SkCanvas.
  SkCanvas* BeginPaint(const gfx::Rect& damage_rect) override;

  // Called on FinishDrawingFrame. The compositor will no longer mutate the the
  // SkCanvas instance returned by |BeginPaint| and should discard any reference
  // that it holds to it.
  void EndPaint() override;

  // Discard the backing buffer in the surface provided by this instance.
  void DiscardBackbuffer() override {}

  // Ensures that there is a backing buffer available on this instance.
  void EnsureBackbuffer() override {}

  // VSyncProvider used to update the timer used to schedule draws with the
  // hardware vsync. Return NULL if a provider doesn't exist.
  gfx::VSyncProvider* GetVSyncProvider() override;

  void SetResizeCallback(::base::Callback<void(void)> closure);
 protected:
  gfx::Size viewport_pixel_size_;
  float scale_factor_;
  gfx::Rect damage_rect_;
  std::unique_ptr<gfx::VSyncProvider> vsync_provider_;
  ::base::Callback<void(void)> resize_closure_;
 private:
  DISALLOW_COPY_AND_ASSIGN(OutputDevice);
};

class TextureOutputDevice : public OutputDevice {
 public:
  TextureOutputDevice(views::Widget* widget);
  ~TextureOutputDevice() override;

  void Resize(const gfx::Size& pixel_size, float scale_factor) override;
  SkCanvas* BeginPaint(const gfx::Rect& damage_rect) override;
  void EndPaint() override;

  Canvas2DPtr GetCanvas() { return canvas2d_;}
 protected:
  Canvas2DPtr canvas2d_;
  Context2D* context2d_;
  views::Widget* widget_;
 private:
  DISALLOW_COPY_AND_ASSIGN(TextureOutputDevice);
};

// class SwapchainOutputDevice
class SwapchainOutputDevice : public TextureOutputDevice {
 public:
  SwapchainOutputDevice(views::Widget* widget);
  ~SwapchainOutputDevice() override;

  void Resize(const gfx::Size& pixel_size, float scale_factor) override;
  SkCanvas* BeginPaint(const gfx::Rect& damage_rect) override;
  void EndPaint() override;
 protected:
  std::unique_ptr<SwapchainContext> widget_context_;
  bool auto_present_;
 private:
  DISALLOW_COPY_AND_ASSIGN(SwapchainOutputDevice);
};
}  // namespace azer
