#include "azer/ui/compositor/compositor.h"

#include "azer/ui/compositor/layer.h"
#include "azer/render/render.h"
#include "azer/render/context2d.h"

namespace azer {
namespace compositor {

Compositor::Compositor()
    : host_(NULL) {
  
}

Compositor::~Compositor() {
}

void Compositor::SetTreeHost(LayerTreeHost* host) {
  host_ = host;
  host_->SetClient(this);
}

void Compositor::DoComposite() {
  RenderSystem* rs = RenderSystem::Current();
  rs->GetContext2D()->finish();
  Layer* root = host_->root();
  gfx::Rect rect(root->bounds());
  CompositeLayer(root, rect);
}

void Compositor::OnResize(const gfx::Size& size) {
  RenderSystem* rs = RenderSystem::Current();
  azer::Texture::Options rdopt;
  rdopt.size = size;
  rdopt.target = (azer::Texture::BindTarget)
      (azer::Texture::kRenderTarget | azer::Texture::kShaderResource);
  renderer_.reset(rs->CreateRenderer(rdopt));
}

gfx::Rect Compositor::CalcRect(Layer* layer, const gfx::Rect& rect) {
  gfx::Point pt(rect.origin().x() + layer->position().x(),
                rect.origin().y() + layer->position().y());
  gfx::Rect rc(pt, layer->bounds().size());

  return std::move(gfx::IntersectRects(rect, rc));
}

void Compositor::ScheduleDraw() {
  auto list = root_layer()->children();
  for (auto iter = list.begin(); iter != list.end(); ++iter) {
    Layer* layer = (*iter);
    if (layer->visible()) {
      layer->Redraw();
    }
  }
}

void Compositor::CompositeLayer(Layer* parent, const gfx::Rect& prect) {
  parent->SortChildren();
  auto list = parent->children();
  for (auto iter = list.begin(); iter != list.end(); ++iter) {
    Layer* layer = (*iter);
    if (layer->visible()) {
      gfx::Rect rc = std::move(CalcRect(layer, prect));
      layer->Render(renderer_.get(), rc);
      CompositeLayer(layer, rc);
    }
  }
}

TexturePtr& Compositor::GetOutputTexture() {
  RenderTargetPtr target = renderer_->GetRenderTarget();
  return target->GetTexture();
}
}  // namespace compositor
}  // namespace azer
