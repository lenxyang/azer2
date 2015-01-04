#include "ui/adapter/output_surface.h"

#include "azer/ui/adapter/output_device.h"

namespace azer {
Azer2DOutputSurface::Azer2DOutputSurface(scoped_ptr<cc::SoftwareOutputDevice> device,
                                         RenderSystem* rs)
    : cc::OutputSurface(device.Pass())
    , weak_ptr_factory_(this)
    , render_system_(rs)
    , renderer_(rs->GetDefaultRenderer()) {
  overlay_.reset(rs->CreateOverlay(gfx::RectF(-1.0f, -1.0f, 1.0f, 1.0f)));
  overlay_->EnableBlending(true);
}
  
Azer2DOutputSurface::~Azer2DOutputSurface() {
}

// cc::OutputSurface implementation
void Azer2DOutputSurface::SwapBuffers(cc::CompositorFrame* frame) {
  client_->DidSwapBuffers();

  render_system_->GetContext2D()->finish();
  Azer2DDevice* device = (Azer2DDevice*)(this->software_device());
  Canvas2DPtr canvas = device->GetCanvas();
  overlay_->SetTexture(canvas->GetTexture());
  overlay_->Render(renderer_);
}
}  // namespace azer
