#pragma once

#include <vector>
#include "base/observer_list.h"
#include "azer/render/frame_args.h"
#include "azer/render/renderer.h"
#include "azer/render/renderable_object.h"

#include "azer/scene/scene_node.h"

namespace azer {

class SceneRender;
class SceneSurroundings;

class SceneRenderObserver {
 public:
  virtual void OnSceneUpdateBegin(SceneRender* scene_render) {}
  virtual void OnSceneUpdateEnd(SceneRender* scene_render) {}
};

class SceneRender {
 public:
  explicit SceneRender(SceneNodePtr node);
  virtual ~SceneRender();

  void Render(const FrameArgs& frame, Renderer* renderer);

  const FrameArgs* frame() { return frame_;}
  SceneSurroundings* mutable_surroundings() { return surroundings_.get();}

  void AddObserver(SceneRenderObserver* observer);
  void RemoveObserver(SceneRenderObserver* observer);
  bool HasObserver(SceneRenderObserver* observer);
  
  virtual void PrepareRender(SceneNode* node) = 0;
  virtual void Render(int target, Renderer* render) = 0;

  virtual void Update(const FrameArgs& frame);
 protected:
  virtual void OnUpdateBegin() {}
  virtual void OnUpdateEnd() {}
  void UpdateSceneRecusive(SceneSurroundings* env, SceneNode* node);
  void RenderObj(RenderableObjectPtr obj, SceneSurroundings* env, SceneNode* node);
  SceneNodePtr node_;
  const FrameArgs* frame_;
  scoped_ptr<SceneSurroundings> surroundings_;
  ObserverList<SceneRenderObserver> observers_;
  DISALLOW_COPY_AND_ASSIGN(SceneRender);
};

class SimpleSceneRender : public SceneRender {
 public:
  SimpleSceneRender(SceneNodePtr node);

  void PrepareRender(SceneNode* node) override;
  void Render(int target, Renderer* render) override;
 private:
  void OnUpdateBegin() override;
  std::vector<RenderableObjectPtr> vec_;
  DISALLOW_COPY_AND_ASSIGN(SimpleSceneRender);
};
}  // namespace azer
