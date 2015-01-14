#include "azer/ui/compositor/texture_layer.h"


#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/rect_f.h"
#include "base/logging.h"
#include "azer/ui/compositor/layer_tree_host.h"
#include "azer/render/render.h"

namespace azer {
namespace compositor {
TextureLayer::TextureLayer(LayerDelegate* delegate)
    : Layer(delegate) {
}

TextureLayer::~TextureLayer() {
}

void TextureLayer::Render(Renderer* renderer, const gfx::Rect& parent_rc) {
  if (texture_.get()) {
    OverlayPtr& overlay = host_->GetOverlay();
    DCHECK(overlay.get() != NULL);
    gfx::Rect intersect = gfx::IntersectRects(parent_rc, bounds());
    gfx::RectF tex_rc((intersect.x() - bounds().x()) / (float)bounds().width(),
                        (intersect.y() - bounds().y()) / (float)bounds().height(),
                        (intersect.width()) / (float)bounds().width(),
                        (intersect.height()) / (float)bounds().height());
    tex_rc = gfx::RectF(overlay_rc.x() * 2.0f - 1.0f,
                        overlay_rc.y() * 2.0f - 1.0f,
                        overlay_rc.width(),
                        overlay_rc.height());
  }
}

void TextureLayer::UpdateOverlayCoord() {
  
}

}  // namespace compositor
}  // namespace azer
