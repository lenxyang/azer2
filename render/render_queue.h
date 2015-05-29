#pragma once

#include <deque>
#include <string>
#include <vector>


#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/renderable_object.h"

namespace azer {

/**
 * RenderQueue 将所有具有相同 Effect 的类型放在一个队列当中进行渲染
 */
class RenderQueue : public RenderableObject {
 public:
  RenderQueue();
  virtual ~RenderQueue();

  EffectPtr& GetEffect();
  void SetEffect(EffectPtr effect);

  void AddObject(const RenderableObjectPtr& object);
  bool RemoveObject(const RenderableObjectPtr& object);
  bool Exists(const RenderableObjectPtr& object);
  void Clear() { queue_.clear();}

  // override from Rendererable
  // void Render(const FrameData& frame, Renderer* renderer) override;
  // EffectParamsProvider* GetEffectParamsProvider() override;
 private:
  EffectPtr effect_;
  std::vector<RenderableObjectPtr> queue_;
  DISALLOW_COPY_AND_ASSIGN(RenderQueue);
};

typedef scoped_refptr<RenderQueue> RenderQueuePtr;
}  // namespace azer
