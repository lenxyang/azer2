#include "azer/ui/adapter/output_surface.h"

#include "SkCanvas.h"
#include "SkImageInfo.h"
#include "SkString.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/logging.h"
#include "base/command_line.h"
#include "base/trace_event/trace_event.h"
#include "cc/base/switches.h"
#include "azer/base/image.h"
#include "azer/render/render_system.h"
#include "azer/ui/adapter/output_device.h"

namespace azer {
Azer2DOutputSurface::Azer2DOutputSurface(
    std::unique_ptr<cc::SoftwareOutputDevice> device,
    base::SingleThreadTaskRunner* task_runner)
    : cc::OutputSurface(std::move(device)),
      weak_ptr_factory_(this) {
  capabilities_.adjust_deadline_for_parent = false;
  synthetic_begin_frame_source_.reset(new cc::SyntheticBeginFrameSource(
      task_runner,
      cc::BeginFrameArgs::DefaultInterval()));
  use_begin_frame_scheduling_ = false;
  

  TextureOutputDevice* azer_device = GetOutputDevice();
  base::Callback<void(void)> callback = 
      ::base::Bind(&Azer2DOutputSurface::OnDeviceResize, base::Unretained(this));
  azer_device->SetResizeCallback(callback);

  TRACE_EVENT_OBJECT_CREATED_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("azer.ui"), "azer::Azer2DOutputSurface", id_);
}

Azer2DOutputSurface::~Azer2DOutputSurface() {
  TRACE_EVENT0("azer", "Azer2DOutputSurface::~Azer2DOutputSurface()");
  TRACE_EVENT_OBJECT_DELETED_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("azer.ui"), "azer::Azer2DOutputSurface", id_);
}

void Azer2DOutputSurface::OnDeviceResize() {
  TRACE_EVENT0("azer", "Azer2DOutputSurface::OnDeviceResize()");
  TextureCopy();
}

void Azer2DOutputSurface::TextureCopy() {
  TextureOutputDevice* device = GetOutputDevice();
  azer::Canvas2DPtr ptr = device->GetCanvas();
  texture_ = ptr->GetTexture();
}


// cc::OutputSurface implementation
bool Azer2DOutputSurface::BindToClient(cc::OutputSurfaceClient* client) {
  if (!OutputSurface::BindToClient(client))
    return false;

  // Pass begin frame source up to Display to use for DisplayScheduler.
  client->SetBeginFrameSource(synthetic_begin_frame_source_.get());

  return true;
}

void Azer2DOutputSurface::SwapBuffers(cc::CompositorFrame* frame) {
  TRACE_EVENT0("azer", "Azer2DOutputSurface::SwapBuffers()");
  TextureCopy();
  
  client_->DidSwapBuffers();
  OutputSurface::OnSwapBuffersComplete();
}

TextureOutputDevice* Azer2DOutputSurface::GetOutputDevice() {
  TextureOutputDevice* device = (TextureOutputDevice*)(this->software_device());
  return device;
}

}  // namespace azer
