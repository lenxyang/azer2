#include "azer/ui/compositor/compositor.h"

#include "azer/ui/compositor/layer.h"

#include "azer/render/render_system.h"
#include "azer/render/app_context.h"
#include "azer/render/layers/layer.h"
#include "azer/render/layers/layer_tree_host.h"


namespace ui {

Compositor::Compositor(gfx::AcceleratedWidget widget) 
    : root_layer_(NULL) {
  AppContext::CreateInstance(widget);

  host_.reset(new azer::layers::LayerTreeHost(size));
  compositor_.reset(new azer::layers::Compositor);
}

Compositor::~Compositor() {
  compositor_.reset();
  host_.reset();
}

void Compositor::ScheduleFullRedraw() {
}

void Compositor::ScheduleRedrawRect(const gfx::Rect& damage_rect) {
}

void Compositor::SetRootLayer(Layer* root_layer) {
  DCHECK(!host_->root());
  host_->SetRoot(root_layer->layer());
  host_->SetCompositor(compositor_.get());
  compositor_->SetTreeHost(host_.get());
  root_layer->compositor_ = this;
  root_layer_ = root_layer;
}

void Compositor::SetScaleAndSize(float scale, const gfx::Size& size_in_pixel) {
  DCHECK(host_.get());
  host_->resize(size_in_pixel);
}

void Compositor::SetBackgroundColor(SkColor color) {
  background_color_ = color;
}

gfx::Size Compositor::size() const { 
  if (root_layer()) {
    return root_layer()->bounds().size();
  } else {
    return gfx::Size();
  }
}

azer::SwapChainPtr& Compositor::GetSwapChain() { 
  return swapchain_;
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

void Compositor::DoRender() {
  azer::AppContext* app = azer::AppContext::GetInstance();
  app->RenderUI(compositor_->GetOutputTexture());
}

void Compositor::DoDraw() {
  for (auto iter = need_redraw_.begin(); iter != need_redraw_.end(); ++iter) {
    Layer* layer = (*iter);
    if (layer->visible()) {
      layer->Redraw();
    }
  }
  need_redraw_.clear();
}

void Compositor::DoComposite() {
  DoDraw();
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
void Compositor::OnLayerNeedRedraw(Layer* layer) {
  AddNeedRedrawLayer(layer);
}

void Compositor::OnLayerAttached(Layer* layer) {
  AddNeedRedrawLayer(layer);
}

void Compositor::OnLayerRemoved(Layer* layer) {
  TryRemoveNeedRedrawLayer(layer);
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
gfx::Rect Compositor::CalcRect(Layer* layer, const gfx::Rect& rect) {
  gfx::Point pt(rect.origin().x() + layer->position().x(),
                rect.origin().y() + layer->position().y());
  gfx::Rect rc(pt, layer->bounds().size());

  return std::move(gfx::IntersectRects(rect, rc));
}

}  // namespace ui
