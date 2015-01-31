#include "azer/render/compositor/canvas_layer.h"

#include "base/logging.h"
#include "ui/gfx/canvas.h"
#include "azer/render/render.h"
#include "azer/render/context2d.h"
#include "azer/render/compositor/layer_delegate.h"


namespace azer {
namespace compositor {

CanvasLayer::CanvasLayer()
    : TextureLayer(kCanvasLayer) {
}

CanvasLayer::~CanvasLayer() {
}

void CanvasLayer::SetBounds(const gfx::Rect& bounds) {
  // before the layer putted into tree host, it cannot be drawn
  // so need not to create canvas
  if (GetTreeHost()) {
    DCHECK(!bounds.IsEmpty());
    RenderSystem* rs = RenderSystem::Current();
    canvas_.reset(rs->GetContext2D()->CreateCanvas(bounds.width(), 
                                                   bounds.height()));
  }

  TextureLayer::SetBounds(bounds);
}

void CanvasLayer::Redraw() {
  if (delegate_) {
    SkCanvas* skcanvas = canvas_->BeginPaint();
    scoped_ptr<gfx::Canvas> canvas(
        gfx::Canvas::CreateCanvasWithoutScaling(skcanvas, 1.0f));
    delegate_->OnPaintLayer(canvas.get());
    canvas->DrawColor(color());
    canvas_->EndPaint();
    texture_ = canvas_->GetTexture();
  }
}
}  // namespace compositor
}  // namespace azer
