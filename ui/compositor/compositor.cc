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
  if (host_) {
    host_->SetCompositor(NULL);
  }
}

void Compositor::SetTreeHost(LayerTreeHost* host) {
  if (host_) {
    host_->SetCompositor(NULL);
  }
  
  host_ = host;
  if (host_) {
    host_->SetCompositor(this);
  }
}

void Compositor::DoComposite() {
  RenderSystem* rs = RenderSystem::Current();
  rs->GetContext2D()->finish();
  renderer_->Use();
  renderer_->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
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
  if (!overlay_.get()) {
    overlay_.reset(rs->CreateOverlay());
  }
}

gfx::Rect Compositor::CalcRect(Layer* layer, const gfx::Rect& rect) {
  gfx::Point pt(rect.origin().x() + layer->position().x(),
                rect.origin().y() + layer->position().y());
  gfx::Rect rc(pt, layer->bounds().size());

  return std::move(gfx::IntersectRects(rect, rc));
}

void Compositor::ScheduleRedrawRect(const gfx::Rect& damage_rect) {
  if (root_layer()) {
    root_layer()->SchedulePaint(damage_rect);
  }
}

void Compositor::ScheduleDraw() {
  for (auto iter = need_redraw_.begin(); iter != need_redraw_.end(); ++iter) {
    Layer* layer = (*iter);
    if (layer->visible()) {
      layer->Redraw();
    }
  }
}

const gfx::Size& Compositor::size() const {
  if (root_layer()) {
    return root_layer()->bounds().size();
  } else {
    return gfx::Size();
  }
}

void Compositor::SetBackgroundColor(SkColor color) {
  background_color_ = color;
}

void Compositor::CompositeLayer(Layer* parent, const gfx::Rect& prect) {
  auto list = parent->children();
  for (auto iter = list.begin(); iter != list.end(); ++iter) {
    Layer* layer = (*iter);
    gfx::Rect rc = std::move(CalcRect(layer, prect));
    layer->Render(renderer_.get(), rc);
    CompositeLayer(layer, rc);
  }
}

TexturePtr& Compositor::GetOutputTexture() {
  RenderTargetPtr target = renderer_->GetRenderTarget();
  return target->GetTexture();
}

void Compositor::AddNeedRedrawLayer(Layer* layer) {
  need_redraw_.insert(layer);
}

void Compositor::TryRemoveNeedRedrawLayer(Layer* layer) {
  auto iter = need_redraw_.find(layer);
  if (need_redraw_.end() != iter) {
    need_redraw_.erase(iter);
  }
}
}  // namespace compositor
}  // namespace azer
