#pragma once

#include <deque>
#include <string>
#include <vector>

#include "azer/base/export.h"
#include "base/basictypes.h"
#include "azer/render_queue/renderable_object.h"
#include "azer/render_queue/effect_params_provider.h"

namespace azer {

/**
 * RenderQueue 将所有具有相同 Effect 的类型放在一个队列当中进行渲染
 */
class RenderQueue : public ::base::RefCounted<RenderQueue>
                  , public EffectParamsProvider {
 public:
  RenderQueue() {}

  EffectPtr& GetEffect();
  void SetEffect(EffectPtr effect);

  void AddObject(const RenderableObjectPtr& object);
  bool RemoveObject(const RenderableObjectPtr& object);
  bool Exists(const RenderableObjectPtr& object);
  void Clear() { queue_.clear();}

  // override from EffectParamsProvider
  // void Render(FrameData* frame, Renderer* renderer) override;
  // EffectParamsProvider* GetEffectParamsProvider() override;
 private:
  EffectPtr effect_;
  std::vector<RenderableObjectPtr> queue_;
  DISALLOW_COPY_AND_ASSIGN(RenderQueue);
};
}  // namespace azer
