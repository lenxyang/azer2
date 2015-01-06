#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/base/export.h"
#include "azer/render/blending.h"
#include "azer/render/texture.h"

namespace azer {

class Renderer;

namespace ui {

class Layer;
class LayerTreeHost;
typedef std::vector<scoped_refptr<Layer> > LayerList;

class CanvasLayer;
class RendererLayer;

class AZER_EXPORT Layer : public base::RefCounted<Layer> {
 public:
  explicit Layer(Layer* parent);
  explicit Layer(LayerTreeHost* host);
  virtual ~Layer();

  Layer* parent() { return parent_;}
  
  LayerTreeHost* GetTreeHost() { return host_;}

  /**
   * 将 Layer 的内容渲染到 Renderer 当中
   * rect 制定了渲染的目标大小，rect的size有可能小于Layer的size,
   * 此时 layer 仅仅部分内容渲染到制定目标当中
   **/
  virtual void Render(Renderer* renderer, const gfx::RectF& rect) = 0;

  // add, remove child
  void Add(scoped_refptr<Layer>& layer);
  bool Remove(scoped_refptr<Layer>& layer);
  bool IsChild(scoped_refptr<Layer>& layer);

  // move the layer to top or bottom
  void BringToTop(Layer* layer);
  void BringToBottom(Layer* layer);
  int32 order() const { return order_;}
  void SetOrder(int32 o);

  void SetBlending(BlendingPtr& ptr) { blending_ = ptr;}

  void SetBounds(const gfx::Size& bounds);
  const gfx::Size& bounds() const { return bounds_;}

  void SetPosition(const gfx::Point& pt) { position_ = pt;}
  const gfx::Point& position() const { return position_;}

  void SetVisible(bool visible);
  bool visible() const { return visible_;}

  // called when remove from parent
  void RemoveFromParent();
  void OnChildrenOrderChanged();

  TexturePtr& GetContent() { return texture_;}
  // sort the children layer by order
  // 
  void SortChildren();
  LayerList* GetChildren() { return &children_;}
 private:
  BlendingPtr blending_;
  gfx::Size bounds_;
  gfx::Point position_;
  bool visible_;
  int32 order_;
  int32 min_order_;
  int32 max_order_;
  bool sorted_;
  LayerList children_;
  LayerTreeHost* host_;
  Layer* parent_;
  TexturePtr texture_;
  DISALLOW_COPY_AND_ASSIGN(Layer);
};

}  // namespace ui
}  // namespace azer
