// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_COMPOSITOR_H_
#define UI_COMPOSITOR_COMPOSITOR_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/single_thread_task_runner.h"
#include "base/time/time.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/compositor/layer_animator_collection.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/gfx/vector2d.h"

#include "azer/ui/aura/compositor/compositor_export.h"
#include "azer/render/compositor/compositor.h"

class SkBitmap;

namespace base {
class MessageLoopProxy;
class RunLoop;
}

namespace gfx {
class Rect;
class Size;
}

namespace ui {

class Layer;

class COMPOSITOR_EXPORT Compositor {
 public:
  Compositor(gfx::AcceleratedWidget widget);
  ~Compositor();

  // Sets the root of the layer tree drawn by this Compositor. The root layer
  // must have no parent. The compositor's root layer is reset if the root layer
  // is destroyed. NULL can be passed to reset the root layer, in which case the
  // compositor will stop drawing anything.
  // The Compositor does not own the root layer.
  const Layer* root_layer() const { return root_layer_; }
  Layer* root_layer() { return root_layer_; }
  void SetRootLayer(Layer* root_layer);

  // Sets the compositor's device scale factor and size.
  void SetScaleAndSize(float scale, const gfx::Size& size_in_pixel);

  // Returns the size of the widget that is being drawn to in pixel coordinates.
  gfx::Size size() const { return compositor_->size();}

  // Sets the background color used for areas that aren't covered by
  // the |root_layer|.
  void SetBackgroundColor(SkColor color) { compositor_->SetBackgroundColor(color);}

  // Set the visibility of the underlying compositor.
  void SetVisible(bool visible) {}

  // Returns the widget for this compositor.
  gfx::AcceleratedWidget widget() const { return widget_; }

  // Where possible, draws are scissored to a damage region calculated from
  // changes to layer properties.  This bypasses that and indicates that
  // the whole frame needs to be drawn.
  void ScheduleFullRedraw();

  // Schedule redraw and append damage_rect to the damage region calculated
  // from changes to layer properties.
  void ScheduleRedrawRect(const gfx::Rect& damage_rect);
 private:
  gfx::AcceleratedWidget widget_;

  // The root of the Layer tree drawn by this compositor.
  Layer* root_layer_;
  scoped_ptr<azer::compositor::LayerTreeHost> host_;
  scoped_ptr<azer::compositor::Compositor> compositor_;
  DISALLOW_COPY_AND_ASSIGN(Compositor);
};
}  // namespace ui

#endif  // UI_COMPOSITOR_COMPOSITOR_H_
