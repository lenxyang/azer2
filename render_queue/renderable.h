#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render_queue/effect_params_provider.h"

namespace azer {

class FrameData;

/**
 * RenderableObject 
 * RenderableObject 代表可渲染的物体
 * 多阶段渲染，对应一个 MeshObject，可以需要多个 RenderableObject Object，例如
 * 绘制阴影阶段 1 个，真是渲染需要一个， deffered shadering 还需要一个.
 * 每一个阶段对应一个 RenderableObject, 不过他们使用相同的 VertexBuffer 及
 * IndexBuffer
 */
class AZER_EXPORT RenderableObject : ::base::RefCounted<RenderableObject> {
 public:
  RenderableObject();
  virtual ~RenderableObject() {}

  virtual void Render(FrameData* frame, Renderer* renderer) = 0;

  // provide information for effect
  virtual EffectParamsProvider* GetEffectParamsProvider() = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(RenderableObject);
};

typedef ::base::scoped_refptr<RenderableObject> RenderableObjectPtr;

class RenderableObjectGroup {
 public:
 private:
  std::vector<RenderableObjectPtr> objects_;
  DISALLOW_COPY_AND_ASSIGN(RenderableObjectGroup);
};
}  // namespace azer
