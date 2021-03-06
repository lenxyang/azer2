#include "azer/render/layers/canvas_layer.h"

#include "base/logging.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/gfx/canvas.h"
#include "azer/render/render_system.h"
#include "azer/render/context2d.h"
#include "azer/render/layers/layer_delegate.h"


namespace azer {
namespace layers {

CanvasLayer::CanvasLayer()
    : TextureLayer(kCanvasLayer) {
}

CanvasLayer::~CanvasLayer() {
}

void CanvasLayer::OnBoundsChanged() {
  // before the layer putted into tree host, it cannot be drawn
  // so need not to create canvas
  if (!bounds_.IsEmpty()) {
    RenderSystem* rs = RenderSystem::Current();
    DCHECK(rs);
    canvas_ = rs->GetContext2D()->CreateCanvas(bounds_.width(), bounds_.height());
  }

  TextureLayer::OnBoundsChanged();
}

void CanvasLayer::SetBounds(const gfx::Rect& bounds) {
  SetBoundsInternal(bounds);
}

void CanvasLayer::Redraw() {
  if (delegate_ && !bounds_.IsEmpty()) {
    SkCanvas* skcanvas = canvas_->BeginPaint();
    
    std::unique_ptr<gfx::Canvas> canvas(
        gfx::Canvas::CreateCanvasWithoutScaling(skcanvas, 1.0f));
    canvas->DrawColor(color(), SkXfermode::kSrc_Mode);
    delegate_->OnPaintLayer(canvas.get());
    canvas_->EndPaint();
    texture_ = canvas_->GetTexture();
  }
}
}  // namespace layers
}  // namespace azer
