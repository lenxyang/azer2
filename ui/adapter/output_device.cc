#include "azer/ui/adapter/output_device.h"

#include "base/logging.h"
#include "base/trace_event/trace_event.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "ui/gfx/vsync_provider.h"
#include "azer/ui/adapter/util.h"
#include "azer/ui/adapter/output_surface.h"
#include "azer/ui/adapter/swapchain_context.h"
#include "azer/render/render_system.h"

namespace azer {

OutputDevice::OutputDevice()
    : scale_factor_(1.f) {
}

OutputDevice::~OutputDevice() {
}

void OutputDevice::SetResizeCallback(::base::Callback<void(void)> closure) {
  resize_closure_ = closure;
}

void OutputDevice::Resize(const gfx::Size& viewport_pixel_size,
                          float scale_factor) {
  NOTIMPLEMENTED();
}

SkCanvas* OutputDevice::BeginPaint(const gfx::Rect& damage_rect) {
  NOTIMPLEMENTED();
  return NULL;
}

void OutputDevice::EndPaint() {
  NOTIMPLEMENTED();
}

gfx::VSyncProvider* OutputDevice::GetVSyncProvider() {
  return vsync_provider_.get();
}

TextureOutputDevice::TextureOutputDevice(views::Widget* widget)
    : widget_(widget) {
  SetTextureOutputDeviceForWidget(widget, this);
}

TextureOutputDevice::~TextureOutputDevice() {
  SetTextureOutputDeviceForWidget(widget_, NULL);
}

void TextureOutputDevice::Resize(const gfx::Size& viewport_pixel_size,
                          float scale_factor) {
  scale_factor_ = scale_factor;

  if (viewport_pixel_size_ == viewport_pixel_size)
    return;

  azer::RenderSystem* rs = azer::RenderSystem::Current();
  DCHECK(NULL != rs);
  azer::Context2D* context2d = rs->GetContext2D();
  DCHECK(NULL != context2d);
  canvas2d_ = azer::Canvas2DPtr(context2d->CreateCanvas(
      viewport_pixel_size.width(), viewport_pixel_size.height()));
  viewport_pixel_size_ = viewport_pixel_size;
  resize_closure_.Run();
}

SkCanvas* TextureOutputDevice::BeginPaint(const gfx::Rect& damage_rect) {
  DCHECK(canvas2d_);
  damage_rect_ = damage_rect;
  return canvas2d_->BeginPaint();
}

void TextureOutputDevice::EndPaint() {
  canvas2d_->EndPaint();
}

SwapchainOutputDevice::SwapchainOutputDevice(views::Widget* widget)
    : TextureOutputDevice(widget),
      auto_present_(false) {
  widget_context_.reset(new SwapchainContext(widget));
  widget_context_->Reset();
  auto_present_ = !IsRendererWindow(widget);
  TRACE_EVENT_OBJECT_CREATED_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("azer.ui"), "azer::SwapchainOutputDevice", id_);
}

SwapchainOutputDevice::~SwapchainOutputDevice() {
  widget_context_.reset();
  TRACE_EVENT_OBJECT_DELETED_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("azer.ui"), "azer::SwapchainOutputDevice", id_);
}

void SwapchainOutputDevice::Resize(const gfx::Size& viewport_pixel_size,
                                   float scale_factor) {
  TextureOutputDevice::Resize(viewport_pixel_size, scale_factor);
}

SkCanvas* SwapchainOutputDevice::BeginPaint(const gfx::Rect& damage_rect) {
  return TextureOutputDevice::BeginPaint(damage_rect);
}

void SwapchainOutputDevice::EndPaint() {
  TextureOutputDevice::EndPaint();
  if (auto_present_) {
    widget_context_->RenderUI();
    widget_context_->Present();
  }
}
}  // namespace azer
