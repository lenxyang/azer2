// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_H_
#define UI_COMPOSITOR_LAYER_H_

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
  ~Layer() override;

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

  // The parent.
  const Layer* parent() const { return parent_; }
  Layer* parent() { return parent_; }

  LayerType type() const { return type_; }

  // The transform, relative to the parent.
  void SetTransform(const gfx::Transform& transform);
  gfx::Transform transform() const;

  // Return the target transform if animator is running, or the current
  // transform otherwise.
  gfx::Transform GetTargetTransform() const;

  // The bounds, relative to the parent.
  void SetBounds(const gfx::Rect& bounds);
  const gfx::Rect& bounds() const { return bounds_; }

  // Return the target bounds if animator is running, or the current bounds
  // otherwise.
  gfx::Rect GetTargetBounds() const;

  // Sets/gets whether or not drawing of child layers should be clipped to the
  // bounds of this layer.
  void SetMasksToBounds(bool masks_to_bounds);
  bool GetMasksToBounds() const;

  // The opacity of the layer. The opacity is applied to each pixel of the
  // texture (resulting alpha = opacity * alpha).
  float opacity() const;
  void SetOpacity(float opacity);

  // Returns the actual opacity, which the opacity of this layer multipled by
  // the combined opacity of the parent.
  float GetCombinedOpacity() const;

  // Blur pixels by this amount in anything below the layer and visible through
  // the layer.
  int background_blur() const { return background_blur_radius_; }
  void SetBackgroundBlur(int blur_radius);

  // Sets the visibility of the Layer. A Layer may be visible but not
  // drawn. This happens if any ancestor of a Layer is not visible.
  void SetVisible(bool visible);
  bool visible() const { return visible_; }

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
  
  const std::string& name() const { return name_; }
  void set_name(const std::string& name) { name_ = name; }

  // Adds |invalid_rect| to the Layer's pending invalid rect and calls
  // ScheduleDraw(). Returns false if the paint request is ignored.
  bool SchedulePaint(const gfx::Rect& invalid_rect);

  // Schedules a redraw of the layer tree at the compositor.
  // Note that this _does not_ invalidate any region of this layer; use
  // SchedulePaint() for that.
  void ScheduleDraw();

  // Suppresses painting the content by disconnecting |delegate_|.
  void SuppressPaint();
 private:
};
}  // namespace ui

#endif  // UI_COMPOSITOR_LAYER_H_
