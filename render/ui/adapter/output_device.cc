// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/uisbox/adapter/base/output_device.h"

#include "base/logging.h"
#include "cc/output/software_frame_data.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "ui/gfx/vsync_provider.h"

#include "azer/render/context2d.h"
#include "azer/render/canvas2d.h"
#include "azer/render/render_system.h"
#include "azer/uisbox/adapter/base/widget_util.h"

TextureOutputDevice::TextureOutputDevice(azer::RenderSystem* render_system,
                                         views::Widget* widget)
    : scale_factor_(1.f)
    , render_system_(render_system)
    , widget_(widget) {
  SetTextureOutputDeviceForWidget(widget, (TextureOutputDevice*)this);
}

TextureOutputDevice::~TextureOutputDevice() {
  SetTextureOutputDeviceForWidget(widget_, NULL);
}

void TextureOutputDevice::SetResizeCallback(::base::Callback<void(void)> closure) {
  resize_closure_ = closure;
}

void TextureOutputDevice::Resize(const gfx::Size& viewport_pixel_size,
                          float scale_factor) {
  scale_factor_ = scale_factor;

  if (viewport_pixel_size_ == viewport_pixel_size)
    return;

  DCHECK(NULL != render_system_);
  azer::Context2D* context2d = render_system_->GetContext2D();
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

void TextureOutputDevice::EndPaint(cc::SoftwareFrameData* frame_data) {
  DCHECK(frame_data);
  frame_data->id = 0;
  frame_data->size = viewport_pixel_size_;
  frame_data->damage_rect = damage_rect_;
  canvas2d_->EndPaint();
}

void TextureOutputDevice::CopyToPixels(const gfx::Rect& rect, void* pixels) {
  DCHECK(canvas_);
  SkCanvas* canvas = canvas2d_->GetSkCanvas();
  SkImageInfo info = SkImageInfo::MakeN32Premul(rect.width(), rect.height());
  canvas->readPixels(info, pixels, info.minRowBytes(), rect.x(), rect.y());
}

void TextureOutputDevice::Scroll(const gfx::Vector2d& delta,
                          const gfx::Rect& clip_rect) {
  NOTIMPLEMENTED();
}

void TextureOutputDevice::ReclaimSoftwareFrame(unsigned id) {
  NOTIMPLEMENTED();
}

gfx::VSyncProvider* TextureOutputDevice::GetVSyncProvider() {
  return vsync_provider_.get();
}

