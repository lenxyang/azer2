#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/transform.h"
#include "azer/base/export.h"

namespace gfx {
class Transform;
}

namespace azer {

class Renderer;

namespace compositor {

class Layer;
typedef std::vector<Layer*> LayerList;

class CanvasLayer;
class RendererLayer;
class LayerDelegate;

class AZER_EXPORT Layer {
 public:
  explicit Layer(LayerDelegate* delegate);
  virtual ~Layer();

  Layer* parent() { return parent_;}
  const Layer* parent() const { return parent_;}
  
  LayerTreeHost* GetTreeHost() { return host_;}

  /**
   * 将 Layer 的内容渲染到 Renderer 当中
   * rect 制定了渲染的目标大小，rect的size有可能小于Layer的size,
   * 此时 layer 仅仅部分内容渲染到制定目标当中
   **/
  virtual void Render(Renderer* renderer, const gfx::RectF& rect) = 0;

  // add, remove child
  void Add(Layer* layer);
  bool Remove(Layer* layer);
  bool IsChild(Layer* layer);

  // move the layer to top or bottom
  void BringToTop(Layer* layer);
  void BringToBottom(Layer* layer);
  int32 order() const { return order_;}
  void SetOrder(int32 o);

  void SetBounds(const gfx::Rect& bounds);
  const gfx::Rect& bounds() const { return bounds_;}

  void SetPosition(const gfx::Point& pt) { position_ = pt;}
  const gfx::Point& position() const { return position_;}

  void SetVisible(bool visible);
  bool visible() const { return visible_;}

  // called when remove from parent
  void RemoveFromParent();
  void OnChildrenOrderChanged();

  // sort the children layer by order
  // 
  void SortChildren();
  LayerList* GetChildren() { return &children_;}

  // Converts a point from the coordinates of |source| to the coordinates of
  // |target|. Necessarily, |source| and |target| must inhabit the same Layer
  // tree.
  static void ConvertPointToLayer(const Layer* source,
                                  const Layer* target,
                                  gfx::Point* point);
  // Converts a transform to be relative to the given |ancestor|. Returns
  // whether success (that is, whether the given ancestor was really an
  // ancestor of this layer).
  bool GetTargetTransformRelativeTo(const Layer* ancestor,
                                    gfx::Transform* transform) const;
 private:
  bool ConvertPointForAncestor(const Layer* ancestor, gfx::Point* point) const;
  bool ConvertPointFromAncestor(const Layer* ancestor, gfx::Point* point) const;

  LayerDelegate* delegate_;
  gfx::Rect bounds_;
  gfx::Point position_;
  bool visible_;
  int32 order_;
  int32 min_order_;
  int32 max_order_;
  bool sorted_;
  LayerList children_;
  Layer* parent_;
  DISALLOW_COPY_AND_ASSIGN(Layer);
};
}  // namespace compositor
}  // namespace azer
