#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "third_party/skia/include/core/SkColor.h"
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
  enum LayerType {
    kNotDrawnLayer,
    kCanvasLayer,
    kRendererLayer,
    kBitmapLayer,
  };

  explicit Layer(LayerType type);
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
  void set_name(const std::string& name) { name_ = name;}

  LayerDelegate* delegate() { return delegate_; }
  void set_delegate(LayerDelegate* delegate) { delegate_ = delegate; }

  // add, remove child
  void Add(Layer* layer);
  bool Remove(Layer* layer);
  bool IsChild(Layer* layer);

  // Sets the layer's fill color.  May only be called for LAYER_SOLID_COLOR.
  void SetColor(SkColor color);
  SkColor color() { return color_;}

  // The opacity of the layer. The opacity is applied to each pixel of the
  // texture (resulting alpha = opacity * alpha).
  float opacity() const { return 1.0f;}
  void SetOpacity(float opacity) { }

  // Adds |invalid_rect| to the Layer's pending invalid rect and calls
  // ScheduleDraw(). Returns false if the paint request is ignored.
  bool SchedulePaint(const gfx::Rect& invalid_rect);

  // Schedules a redraw of the layer tree at the compositor.
  // Note that this _does not_ invalidate any region of this layer; use
  // SchedulePaint() for that.
  void ScheduleDraw();

  void StackAtTop(Layer* child);
  void StackAbove(Layer* child, Layer* target);
  void StackBelow(Layer* child, Layer* target);
  void StackAtBottom(Layer* child);

  virtual void SetBounds(const gfx::Rect& bounds) = 0;
  const gfx::Rect& bounds() const { return bounds_;}

  /**
   * 虽然设置了 bounds, 但不是所有的 bounds 都能够显示的，
   * 如果超出了 parent->bounds, 就不能显示了， target_bounds
   * 就是未超出的部分，或者有效部分
   */
  const gfx::Rect& GetTargetBounds() const { return target_bounds_;}
  bool GetTargetVisibility() const;

  void SetPosition(const gfx::Point& pt) { position_ = pt;}
  const gfx::Point& position() const { return position_;}

  void SetVisible(bool visible);
  bool visible() const { return visible_;}

  // called when remove from parent
  virtual void OnRemoveFromParent() {}
  virtual void OnStackingChanged() {}

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
  void StackRelativeTo(Layer* child, Layer* other, bool above);

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
  LayerList children_;
  Layer* parent_;

  SkColor color_;
  LayerType type_;
  LayerObserverList observers_;
  friend class LayerTreeHost;
  DISALLOW_COPY_AND_ASSIGN(Layer);
};
}  // namespace compositor
}  // namespace azer
