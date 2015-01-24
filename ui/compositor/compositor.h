#pragma once

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/base/export.h"
#include "azer/render/renderer.h"
#include "azer/ui/compositor/layer_tree_host.h"

namespace azer {

class RenderSystem;

namespace compositor {

class Layer;
class LayerTreeHost;

class AZER_EXPORT Compositor : public LayerTreeHostClient {
 public:
  Compositor();
  ~Compositor();

  void SetTreeHost(LayerTreeHost* host);

  // merge all output into output texture
  void DoComposite();
  /* ScheduleDraw 是异步调用，返回式绘制可能尚未完成
  * （例如 GL 调用必须等到 glFinish 返回之后才能确保所有操作已经完成 ）
  * (save the layer need to redraw, and just draw them)
  */
  void ScheduleDraw();

  // get all layer in damage_rect and repaint them
  void ScheduleRedrawRect(const gfx::Rect& damage_rect);

  TexturePtr& GetOutputTexture();
  RendererPtr& GetRenderer();
  OverlayPtr& overlay() { return overlay_;}

  const Layer* root_layer() const { return host_->root();}
  Layer* root_layer() { return host_->root();}

  LayerTreeHost* GetTreeHost() { return host_;}

  // Returns the size of the widget that is being drawn to in pixel coordinates.
  gfx::Size size() const;

  // Sets the background color used for areas that aren't covered by
  // the |root_layer|.
  void SetBackgroundColor(SkColor color);
 protected:
  void AddNeedRedrawLayer(Layer* layer);
  void TryRemoveNeedRedrawLayer(Layer* layer);

  void OnResize(const gfx::Size& size) override;
  /**
   * Layer 仅保留相对于父窗口的坐标和大小, 此函数负责计算
   * Layer 的窗口坐标， 参数 rect 为父窗口的窗口坐标
   * Notes: 如果子窗口的坐标超出了父窗口的坐标，
   * 那么 rect 的 size 小于 layer 的 size
   */
  gfx::Rect CalcRect(Layer* layer, const gfx::Rect& rect);

  /**
   * 递归的调用每一层的 Layer 并渲染到一个 texture 当中
   * rect 为当前 layer 的位置(先对于整个Texture来说)
   */
  void CompositeLayer(Layer* layer, const gfx::Rect& rect);

  std::set<Layer*> need_redraw_;
  LayerTreeHost* host_;
  OverlayPtr overlay_;
  RendererPtr renderer_;
  SkColor background_color_;
  friend class LayerTreeHost;
  DISALLOW_COPY_AND_ASSIGN(Compositor);
};

}  // namespace compositor
}  // namespace azer
