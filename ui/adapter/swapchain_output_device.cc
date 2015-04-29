// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/adapter/swapchain_output_device.h"

#include "base/logging.h"
#include "base/bind.h"
#include "base/callback.h"
#include "cc/output/software_frame_data.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "ui/gfx/vsync_provider.h"
#include "ui/views/widget/widget.h"
#include "azer/render/context2d.h"
#include "azer/render/canvas2d.h"
#include "azer/ui/widget_renderer_context.h"

namespace azer {

SwapchainOutputDevice::SwapChainDeviceRenderer::
      SwapChainDeviceRenderer(WidgetRendererContext* context) 
    : stop_(false)
    , first_post_task_(true)
    , context_(context) {
}

void SwapchainOutputDevice::SwapChainDeviceRenderer::PostTask() {
  if (first_post_task_) {
    PostProc();
  }
}

void SwapchainOutputDevice::SwapChainDeviceRenderer::Stop() {
  stop_ = true;
}

void SwapchainOutputDevice::SwapChainDeviceRenderer::RenderProc() {
  DCHECK(context_);
  context_->RenderUI();
  context_->Present();
}

void SwapchainOutputDevice::SwapChainDeviceRenderer::PostProc() {
  if (!stop_) {
    RenderProc();
    ::base::MessageLoop::ScopedNestableTaskAllower
          allow(::base::MessageLoop::current());
    ::base::Closure closure = ::base::Bind(
        &SwapchainOutputDevice::SwapChainDeviceRenderer::PostProc, this);
    ::base::MessageLoop::current()->PostTask(FROM_HERE, closure);
  }
}

SwapchainOutputDevice::SwapchainOutputDevice(azer::RenderSystem* render_system,
                                             views::Widget* widget)
    : TextureOutputDevice(render_system, widget) {
  widget_context_.reset(new WidgetRendererContext(widget, NULL));
  render_util_ = new SwapChainDeviceRenderer(widget_context_.get());
}

SwapchainOutputDevice::~SwapchainOutputDevice() {
  render_util_->Stop();
  widget_context_.reset();
}

void SwapchainOutputDevice::Resize(const gfx::Size& viewport_pixel_size,
                                   float scale_factor) {
  TextureOutputDevice::Resize(viewport_pixel_size, scale_factor);
}

SkCanvas* SwapchainOutputDevice::BeginPaint(const gfx::Rect& damage_rect) {
  return TextureOutputDevice::BeginPaint(damage_rect);
}

void SwapchainOutputDevice::EndPaint(cc::SoftwareFrameData* frame_data) {
  TextureOutputDevice::EndPaint(frame_data);
  // render_util_->PostTask();
  widget_context_->RenderUI();
  widget_context_->Present();
}


}  // namespace azer
