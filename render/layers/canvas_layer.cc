#include "azer/render/layers/canvas_layer.h"

#include "base/logging.h"
#include "ui/gfx/canvas.h"
#include "azer/render/render.h"
#include "azer/render/context2d.h"
#include "azer/render/layers/layer_delegate.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"


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
    canvas_.reset(rs->GetContext2D()->CreateCanvas(bounds_.width(), 
                                                   bounds_.height()));
  }

  TextureLayer::OnBoundsChanged();
}

void CanvasLayer::SetBounds(const gfx::Rect& bounds) {
  SetBoundsInternal(bounds);
}

void CanvasLayer::Redraw() {
  if (delegate_ && !bounds_.IsEmpty()) {
    SkCanvas* skcanvas = canvas_->BeginPaint();
    scoped_ptr<gfx::Canvas> canvas(
        gfx::Canvas::CreateCanvasWithoutScaling(skcanvas, 1.0f));
    delegate_->OnPaintLayer(canvas.get());
    canvas->DrawColor(color());
    canvas_->EndPaint();
    texture_ = canvas_->GetTexture();
    /*
    canvas_->Save(::base::FilePath(::base::UTF8ToWide(
        ::base::StringPrintf("%s.png", name().c_str()))));
    */
  }
}
}  // namespace layers
}  // namespace azer
