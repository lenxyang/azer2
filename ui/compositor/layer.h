#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/transform.h"

#include "azer/base/export.h"
#include "azer/ui/compositor/layer_observer.h"

namespace gfx {
class Transform;
}

namespace azer {

class Renderer;

namespace compositor {

class Layer;
class LayerTreeHost;
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

  void AddObserver(LayerObserver* observer);
  void RemoveObserver(LayerObserver* observer);

  /**
   * 将 Layer 的内容渲染到 Renderer 当中
   * parent_rc 为父窗口当前的 rect
   **/
  virtual void Render(Renderer* renderer, const gfx::Rect& parent_rc) = 0;
  virtual void Redraw() = 0;

  const std::string& name() const { return name_;}
  void SetName(const std::string& name) { name_ = name;}

  // add, remove child
  void Add(Layer* layer);
  bool Remove(Layer* layer);
  bool IsChild(Layer* layer);

  // move the layer to top or bottom
  void BringToTop(Layer* layer);
  void BringToBottom(Layer* layer);
  int32 order() const { return order_;}
  void SetOrder(int32 o);

  virtual void SetBounds(const gfx::Rect& bounds) = 0;
  const gfx::Rect& bounds() const { return bounds_;}

  /**
   * 虽然设置了 bounds, 但不是所有的 bounds 都能够显示的，
   * 如果超出了 parent->bounds, 就不能显示了， target_bounds
   * 就是未超出的部分，或者有效部分
   */
  const gfx::Rect& target_bounds() const { return target_bounds_;}

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
  LayerList& children() { return children_;}
  const LayerList& children() const { return children_;}

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
  void SetNeedRedraw(const gfx::Rect& rect);
 protected:
  void SetTreeHost(LayerTreeHost* host);
  bool ConvertPointForAncestor(const Layer* ancestor, gfx::Point* point) const;
  bool ConvertPointFromAncestor(const Layer* ancestor, gfx::Point* point) const;

  /**
   * 在更改窗口的 bounds 时，需要重新计算 overly_bounds 和 tex_bounds
   * 才外父窗口变化时，子窗口也需要进行计算
   *
   */
  void SetBoundsInternal(const gfx::Rect& new_bounds);
  void OnParentBoundsChanged();
  void OnBoundsChanged();
 protected:
  void CalcTargetBounds();
  void CalcOverlayBounds();
  void CalcTexBounds();
  std::string name_;
  LayerDelegate* delegate_;
  LayerTreeHost* host_;
  gfx::Point position_;
  gfx::Rect bounds_;
  gfx::Rect target_bounds_;
  gfx::RectF overlay_bounds_;
  gfx::RectF tex_bounds_;
  bool visible_;
  int32 order_;
  int32 min_order_;
  int32 max_order_;
  bool sorted_;
  LayerList children_;
  Layer* parent_;

  LayerObserverList observers_;
  friend class LayerTreeHost;
  DISALLOW_COPY_AND_ASSIGN(Layer);
};
}  // namespace compositor
}  // namespace azer
