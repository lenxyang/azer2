// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/adapter/output_device.h"

#include "base/logging.h"
#include "cc/output/software_frame_data.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "ui/gfx/vsync_provider.h"

#include "azer/render/context2d.h"
#include "azer/render/canvas2d.h"

namespace azer {

Azer2DDevice::Azer2DDevice(Context2D* context2d)
    : scale_factor_(1.f)
    , context2d_(context2d) {
}

Azer2DDevice::~Azer2DDevice() {
}

void Azer2DDevice::SetResizeCallback(::base::Callback<void(void)> closure) {
  resize_closure_ = closure;
}

void Azer2DDevice::Resize(const gfx::Size& viewport_pixel_size,
                          float scale_factor) {
  scale_factor_ = scale_factor;

  if (viewport_pixel_size_ == viewport_pixel_size)
    return;

  DCHECK(NULL != context2d_);
  canvas2d_ = Canvas2DPtr(context2d_->CreateCanvas(viewport_pixel_size.width(),
                                                   viewport_pixel_size.height()));
  viewport_pixel_size_ = viewport_pixel_size;
  resize_closure_.Run();
}

SkCanvas* Azer2DDevice::BeginPaint(const gfx::Rect& damage_rect) {
  DCHECK(canvas2d_);
  damage_rect_ = damage_rect;
  return canvas2d_->GetSkCanvas();
}

void Azer2DDevice::EndPaint(cc::SoftwareFrameData* frame_data) {
  DCHECK(frame_data);
  frame_data->id = 0;
  frame_data->size = viewport_pixel_size_;
  frame_data->damage_rect = damage_rect_;
  canvas2d_->GetSkCanvas()->flush();
}

void Azer2DDevice::CopyToPixels(const gfx::Rect& rect, void* pixels) {
  DCHECK(canvas_);
  SkCanvas* canvas = canvas2d_->GetSkCanvas();
  SkImageInfo info = SkImageInfo::MakeN32Premul(rect.width(), rect.height());
  canvas->readPixels(info, pixels, info.minRowBytes(), rect.x(), rect.y());
}

void Azer2DDevice::Scroll(const gfx::Vector2d& delta,
                          const gfx::Rect& clip_rect) {
  NOTIMPLEMENTED();
}

void Azer2DDevice::ReclaimSoftwareFrame(unsigned id) {
  NOTIMPLEMENTED();
}

gfx::VSyncProvider* Azer2DDevice::GetVSyncProvider() {
  return vsync_provider_.get();
}

}  // namespace azer
