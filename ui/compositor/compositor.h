#pragma once

#include "base/basictypes.h"
#include "ui/gfx/geometry/rect.h"
#include "base/memory/ref_counted.h"

#include "azer/base/export.h"
#include "azer/render/renderer.h"
#include "azer/ui/compositor/layer_tree_host.h"

namespace azer {

class RenderSystem;

namespace compositor {

class Layer;
class LayerTreeHost;

class AZER_EXPORT Compositor : public LayerTreeHostClient,
  public ::base::RefCounted<Compositor>  {
 public:
  Compositor();
  ~Compositor();

  // merge all output into output texture
  void DoComposite();
  /* ScheduleDraw 是异步调用，返回式绘制可能尚未完成
  * （例如 GL 调用必须等到 glFinish 返回之后才能确保所有操作已经完成 ）
  * (save the layer need to redraw, and just draw them)
  */
  void ScheduleDraw();

  TexturePtr& GetOutputTexture();
  RendererPtr& GetRenderer();

  const Layer* root_layer() const { return host_->root();}
  Layer* root_layer() { return host_->root();}

  void SetTreeHost(LayerTreeHost* host);
  LayerTreeHost* GetTreeHost() { return host_;}
 protected:
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

  LayerTreeHost* host_;
  RendererPtr renderer_;
  DISALLOW_COPY_AND_ASSIGN(Compositor);
};

typedef scoped_refptr<Compositor> CompositorPtr;
}  // namespace compositor
}  // namespace azer
