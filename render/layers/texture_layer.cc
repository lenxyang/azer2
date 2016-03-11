#include "azer/render/layers/texture_layer.h"


#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/rect_f.h"
#include "base/logging.h"

#include "azer/render/layers/layer_tree_host.h"

namespace azer {
namespace layers {
TextureLayer::TextureLayer(LayerType type)
    : Layer(type) {
}

TextureLayer::~TextureLayer() {
}

void TextureLayer::Render(Renderer* renderer, Overlay* overlay,
                          const gfx::Rect& parent_rc) {
  if (texture_.get()) {
    DCHECK(overlay);
    overlay->SetTexture(texture_);
    overlay->SetTexCoord(tex_bounds_);
    overlay->SetBounds(overlay_bounds_);
    overlay->Render(renderer);
  }
}

void TextureLayer::UpdateOverlayCoord() {
  
}

}  // namespace layers
}  // namespace azer
