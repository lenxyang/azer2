// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_H_
#define UI_COMPOSITOR_LAYER_H_

#include "azer/ui/compositor/layer.h"
#include "azer/ui/aura/layer/compositor.h"
#include "azer/ui/aura/layer/layer_delegate.h"
#include "azer/ui/aura/layer/layer_type.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/transform.h"

class SkCanvas;

namespace ui {

class Compositor;
class LayerAnimator;
class LayerOwner;

class COMPOSITOR_EXPORT Layer {
 public:
  Layer();
  explicit Layer(LayerType type);
  ~Layer();

  // Retrieves the Layer's compositor. The Layer will walk up its parent chain
  // to locate it. Returns NULL if the Layer is not attached to a compositor.
  Compositor* GetCompositor() {
    return const_cast<Compositor*>(
        const_cast<const Layer*>(this)->GetCompositor());
  }
  const Compositor* GetCompositor() const;

  // Called by the compositor when the Layer is set as its root Layer. This can
  // only ever be called on the root layer.
  void SetCompositor(Compositor* compositor);

  LayerDelegate* delegate() { return delegate_; }
  void set_delegate(LayerDelegate* delegate) { delegate_ = delegate; }

  LayerOwner* owner() { return owner_; }

  // Adds a new Layer to this Layer.
  void Add(Layer* child) {layer_->Add(child->layer());}

  // Removes a Layer from this Layer.
  void Remove(Layer* child) { layer_->Remove(child->layer());}

  // Stacks |child| above all other children.
  void StackAtTop(Layer* child) { layer_->StackAtTop(child);}

  // Stacks |child| directly above |other|.  Both must be children of this
  // layer.  Note that if |child| is initially stacked even higher, calling this
  // method will result in |child| being lowered in the stacking order.
  void StackAbove(Layer* child, Layer* other) { layer_->StackAbove(child, other);}

  // Stacks |child| below all other children.
  void StackAtBottom(Layer* child) { layer_->StackAtBottom(child);}

  // Stacks |child| directly below |other|.  Both must be children of this
  // layer.
  void StackBelow(Layer* child, Layer* other) {layer_->StackBelow(child, other);}

  // The parent.
  const Layer* parent() const { return parent();}
  Layer* parent() { return layer_->parent();}

  LayerType type() const { return type_; }

  // The transform, relative to the parent.
  void SetTransform(const gfx::Transform& transform) {}
  gfx::Transform transform() const { return gfx::Transform();}

  // Return the target transform if animator is running, or the current
  // transform otherwise.
  gfx::Transform GetTargetTransform() const { return gfx::Transform();}

  // The bounds, relative to the parent.
  void SetBounds(const gfx::Rect& bounds) { layer_->SetBounds(bounds);}
  const gfx::Rect& bounds() const { return layer_->bounds();}

  // Return the target bounds if animator is running, or the current bounds
  // otherwise.
  gfx::Rect GetTargetBounds() const { return layer_->GetTargetBounds();}

  // Sets/gets whether or not drawing of child layers should be clipped to the
  // bounds of this layer.
  void SetMasksToBounds(bool masks_to_bounds) {}
  bool GetMasksToBounds() const { return false; }

  // The opacity of the layer. The opacity is applied to each pixel of the
  // texture (resulting alpha = opacity * alpha).
  float opacity() const { return layer_->opacity();}
  void SetOpacity(float opacity) { layer_->SetOpacity(opacity);}

  // Returns the actual opacity, which the opacity of this layer multipled by
  // the combined opacity of the parent.
  float GetCombinedOpacity() const;

  // Blur pixels by this amount in anything below the layer and visible through
  // the layer.
  int background_blur() const { return background_blur_radius_; }
  void SetBackgroundBlur(int blur_radius);

  // Return the target opacity if animator is running, or the current opacity
  // otherwise.
  float GetTargetOpacity() const { return opacity();}

  // Sets the visibility of the Layer. A Layer may be visible but not
  // drawn. This happens if any ancestor of a Layer is not visible.
  void SetVisible(bool visible) { layer_->SetVisible(visible);}
  bool visible() const { return layer_->visible();}

  // Returns the target visibility if the animator is running. Otherwise, it
  // returns the current visibility.
  bool GetTargetVisibility() const;

  // Returns true if this Layer is drawn. A Layer is drawn only if all ancestors
  // are visible.
  bool IsDrawn() const ;

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

  // See description in View for details
  void SetFillsBoundsOpaquely(bool fills_bounds_opaquely) {}
  bool fills_bounds_opaquely() const {}

  // Set to true if this layer always paints completely within its bounds. If so
  // we can omit an unnecessary clear, even if the layer is transparent.
  void SetFillsBoundsCompletely(bool fills_bounds_completely) {}
  
  const std::string& name() const { return layer_->name();}
  void set_name(const std::string& name) { layer_->set_name(name);}

  // Adds |invalid_rect| to the Layer's pending invalid rect and calls
  // ScheduleDraw(). Returns false if the paint request is ignored.
  bool SchedulePaint(const gfx::Rect& invalid_rect) {
    layer_->SchedulePaint(invalid_rect);
  }

  // Schedules a redraw of the layer tree at the compositor.
  // Note that this _does not_ invalidate any region of this layer; use
  // SchedulePaint() for that.
  void ScheduleDraw() {layer_->ScheduleDraw();}

  void CompleteAllAnimations() {}

  // Suppresses painting the content by disconnecting |delegate_|.
  void SuppressPaint() {}

  azer::compositor::Layer* layer() { return layer_;}
 private:
  LayerDelegate* delegate_;
  LayerType type_;
  int background_blur_radius_;
  LayerOwner* owner_;

  azer::compositor::Layer* layer_;
  friend class LayerOwner;
};
}  // namespace ui

#endif  // UI_COMPOSITOR_LAYER_H_
