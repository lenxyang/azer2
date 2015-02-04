#include "azer/render/compositor/texture_layer.h"


#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/rect_f.h"
#include "base/logging.h"

#include "azer/render/compositor/compositor.h"
#include "azer/render/compositor/layer_tree_host.h"
#include "azer/render/render.h"

namespace azer {
namespace compositor {
TextureLayer::TextureLayer(LayerType type)
    : Layer(type) {
}

TextureLayer::~TextureLayer() {
}

void TextureLayer::Render(Renderer* renderer, const gfx::Rect& parent_rc) {
  if (texture_.get()) {
    OverlayPtr& overlay = host_->compositor()->overlay();
    DCHECK(overlay.get() != NULL);
    overlay->SetTexture(texture_);
    overlay->SetTexCoord(tex_bounds_);
    overlay->SetBounds(overlay_bounds_);
    overlay->Render(renderer);
  }
}

void TextureLayer::UpdateOverlayCoord() {
  
}

}  // namespace compositor
}  // namespace azer
