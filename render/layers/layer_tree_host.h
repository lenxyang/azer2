#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/base/export.h"
#include "azer/render/render.h"
#include "azer/render/render_system_observer.h"
#include "azer/render/layers/layer_delegate.h"
#include "azer/render/layers/layer_observer.h"
#include "azer/render/layers/layer.h"


namespace azer {

namespace layers {
class Layer;

class AZER_EXPORT LayerTreeHost {
 public:
  LayerTreeHost();
  ~LayerTreeHost();

  Layer* root() { return root_;}
  const Layer* root() const { return root_;}
  void SetRoot(scoped_refptr<Layer> layer);

  void SetBackgroundColor(SkColor color);

  // Set the visibility of the underlying compositor.
  void SetVisible(bool visible) {visible_ = visible;}
  bool visible() const { return visible_;}

  // Sets the compositor's device scale factor and size.
  void SetScaleAndSize(float scale, const gfx::Size& size_in_pixel);

  void SetNeedsRedraw();
  void SetNeedsRedrawRect(const gfx::Rect& damage_rect);
  void SetLayerNeedsRedrawRect(Layer* layer, Layer* rect_layer,
                               const gfx::Rect& damage_rect);
  void SetLayerNeedsRedraw(scoped_refptr<Layer>& layer);
  void SetLayerNeedsRedrawRecusive(scoped_refptr<Layer>& layer);

  struct CompositeArgs {
    ::base::Time time;
    ::base::TimeDelta delta;
  };
  void Composite(const CompositeArgs& args);
  void Composite();

  azer::Texture* GetCompositedTexture();
 private:
  void DoDraw();

  /**
   * Layer 仅保留相对于父窗口的坐标和大小, 此函数负责计算
   * Layer 的窗口坐标， 参数 rect 为父窗口的窗口坐标
   * Notes: 如果子窗口的坐标超出了父窗口的坐标，
   * 那么 rect 的 size 小于 layer 的 size
   */
  gfx::Rect CalcRect(Layer* layer, const gfx::Rect& rect);
  void CompositeRecusive(Layer* layer, const gfx::Rect& rect);

  std::set<LayerPtr> need_redraw_;
  scoped_refptr<Layer> root_;
  bool visible_;
  gfx::Size size_;
  float scale_;

  SkColor background_color_;
  azer::OverlayPtr overlay_;
  azer::RendererPtr renderer_;
  DISALLOW_COPY_AND_ASSIGN(LayerTreeHost);
};

}  // namespace layers
}  // namespace azer
