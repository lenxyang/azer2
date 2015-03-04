#pragma once

#include <deque>
#include <string>
#include <vector>

#include "azer/base/export.h"
#include "base/basictypes.h"
#include "azer/render/renderable_object.h"

namespace azer {

/**
 * RenderQueue 将所有具有相同 Effect 的类型放在一个队列当中进行渲染
 */
class RenderQueue : public ::base::RefCounted<RenderQueue> {
 public:
  RenderQueue() {}

  EffectPtr& GetEffect();
  void SetEffect(EffectPtr effect);

  void AddObject(const RenderableObjectPtr& object);
  bool RemoveObject(const RenderableObjectPtr& object);
  bool Exists(const RenderableObjectPtr& object);
  void Clear() { queue_.clear();}

  void Render();
 private:
  EffectPtr effect_;
  std::vector<RenderableObjectPtr> queue_;
  DISALLOW_COPY_AND_ASSIGN(RenderQueue);
};
}  // namespace azer
