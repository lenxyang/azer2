#include "azer/render/layers/layer_tree_host.h"

#include "third_party/skia/include/core/SkColor.h"
#include "azer/render/layers/layer.h"
#include "azer/render/layers/canvas_layer.h"
#include "azer/render/layers/nodraw_layer.h"
#include "azer/render/layers/renderer_layer.h"
#include "azer/render/render_system.h"
#include "azer/render/context2d.h"

namespace azer {
namespace layers {

LayerTreeHost::LayerTreeHost()
    : root_(NULL)
    , size_(0, 0)
    , scale_(1.0f)
    , background_color_(SK_ColorTRANSPARENT) 
{
}

LayerTreeHost::~LayerTreeHost() {
}

void LayerTreeHost::SetRoot(scoped_refptr<Layer> layer) {
  DCHECK(!root_);
  root_ = layer;
  root_->layer_tree_host_ = this;
  root_->SetBounds(gfx::Rect(0, 0, size_.width(), size_.height()));
}

void LayerTreeHost::SetBackgroundColor(SkColor color) {
  background_color_ = color;
}

void LayerTreeHost::SetScaleAndSize(float scale, const gfx::Size& size_in_pixel) {
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  azer::Texture::Options rdopt;
  rdopt.size = size_in_pixel;
  rdopt.target = (azer::Texture::BindTarget)
      (azer::Texture::kRenderTarget | azer::Texture::kShaderResource);
  renderer_.reset(rs->CreateRenderer(rdopt));
  if (!overlay_.get()) {
    overlay_.reset(rs->CreateOverlay());
  }

  size_ = size_in_pixel;
  scale_ = scale;
}

void LayerTreeHost::SetNeedsRedraw() {
  SetLayerNeedsRedrawRecusive(root_);
}

void LayerTreeHost::SetLayerNeedsRedrawRect(Layer* layer, Layer* rect_layer,
                                            const gfx::Rect& damage_rect) {
  gfx::Point pt(damage_rect.origin());
  Layer::ConvertPointToLayer(rect_layer, layer, &pt);
  gfx::Rect rect(pt, damage_rect.size());
  // mark all layer interacted with rect 'needed render'
  for (auto iter = layer->children().begin();
       iter != layer->children().end(); ++iter) {
    const gfx::Rect& child_bounds = (*iter)->bounds();
    if (child_bounds.Intersects(rect)) {
      SetLayerNeedsRedrawRect((*iter).get(), layer, rect);
    }
  }
}

void LayerTreeHost::SetNeedsRedrawRect(const gfx::Rect& damage_rect) {
  SetLayerNeedsRedrawRect(root_.get(), root_.get(), damage_rect);
}

void LayerTreeHost::SetLayerNeedsRedraw(scoped_refptr<Layer>& layer) {
  need_redraw_.insert(layer);
}

void LayerTreeHost::SetLayerNeedsRedrawRecusive(scoped_refptr<Layer>& layer) {
  SetLayerNeedsRedraw(layer);
  for (auto iter = layer->children().begin(); 
       iter != layer->children().end(); ++iter) {
    SetLayerNeedsRedrawRecusive(*iter);
  }
}

void LayerTreeHost::DoDraw() {
  for (auto iter = need_redraw_.begin(); iter != need_redraw_.end(); ++iter) {
    (*iter)->Redraw();
  }
  need_redraw_.clear();
}

void LayerTreeHost::Composite(const CompositeArgs& args) {}

void LayerTreeHost::Composite() {
  DoDraw();
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  rs->GetContext2D()->finish();
  renderer_->Use();
  renderer_->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  gfx::Rect rect(root()->bounds());
  CompositeRecusive(root_.get(), rect);
}

void LayerTreeHost::CompositeRecusive(Layer* parent, const gfx::Rect& prect) {
  auto list = parent->children();
  for (auto iter = list.begin(); iter != list.end(); ++iter) {
    Layer* layer = (*iter);
    gfx::Rect rc = std::move(CalcRect(layer, prect));
    layer->Render(renderer_.get(), overlay_.get(), rc);
    CompositeRecusive(layer, rc);
  }
}

gfx::Rect LayerTreeHost::CalcRect(Layer* layer, const gfx::Rect& rect) {
  gfx::Point pt(rect.origin().x() + layer->bounds().x(),
                rect.origin().y() + layer->bounds().y());
  gfx::Rect rc(pt, layer->bounds().size());

  return std::move(gfx::IntersectRects(rect, rc));
}

azer::TexturePtr& LayerTreeHost::GetCompositedTexture() {
  DCHECK(renderer_.get());
  return renderer_->GetRenderTarget()->GetTexture();
}
}  // namespace layers
}  // namespace azer
