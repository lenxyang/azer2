#pragma once

#include "azer/base/export.h"

namespace azer {

class SceneNode;
class RenderableObject;

typedef scoped_refptr<SceneNode> SceneNodePtr;
typedef scoped_refptr<RenderableObject> RenderableObjectPtr;


class AZER_EXPORT SceneNodeObserver {
 public:
  SceneNodeObserver();
  virtual ~SceneNodeObserver();

  virtual void OnRenderableObjectAttached(SceneNodePtr& node,
                                          RenderableObjectPtr& object);
  virtual void OnRenderableObjectDetached(SceneNodePtr& node,
                                          RenderableObjectPtr& object);
 private:
  DISALLOW_COPY_AND_ASSIGN(SceneNodeObserver);
};
}  // namespace azer
