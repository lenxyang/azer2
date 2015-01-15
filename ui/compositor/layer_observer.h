#pragma once

#include "base/basictypes.h"
#include "base/observer_list.h"

#include "azer/base/export.h"

namespace azer {
namespace compositor {

class Layer;

class AZER_EXPORT LayerObserver {
 public:
  LayerObserver() {}
  virtual ~LayerObserver() {}

  virtual void OnLayerAttachedOnTree(Layer* layer) {}

  virtual void OnLayerResizing(Layer* layer) {}
  virtual void OnLayerResized(Layer* layer) {}

  virtual void OnLayerDestroying(Layer* layer) {}
 private:
  DISALLOW_COPY_AND_ASSIGN(LayerObserver);
};

typedef ObserverList<LayerObserver, true> LayerObserverList;
}  // namespace compositor
}  // namespace azer
