#pragma once

#include "base/observer_list.h"
#include "azer/base/export.h"

namespace azer {
class Camera;
class FrameArgs;
class RenderTreeBuilderDelegate;
class Renderer;
class RenderNode;
class RenderEnvNode;
class SceneNode;
class SceneTreeRender;
typedef scoped_refptr<RenderNode> RenderNodePtr;

class AZER_EXPORT SceneTreeRenderObserver {
 public:
  virtual void OnFrameUpdateBegin(SceneTreeRender* sr, const FrameArgs& args) {}
  virtual void OnFrameUpdateEnd(SceneTreeRender* sr, const FrameArgs& args) {}
  virtual void OnFrameRenderBegin(SceneTreeRender* sr, Renderer* renderer) {}
  virtual void OnFrameRenderEnd(SceneTreeRender* sr, Renderer* renderer) {}
};

class AZER_EXPORT SceneTreeRender {
 public:
  SceneTreeRender();

  RenderNode* root() { return root_.get();}
  const Camera* camera() const { return camera_;}
  void SetTreeBuildDelegate(scoped_refptr<RenderTreeBuilderDelegate> delegate);
  void Init(SceneNode* root, const Camera* camera);
  void Update(const FrameArgs& args);
  void Render(Renderer* renderer);

  void AddObserver(SceneTreeRenderObserver* observer);
  void RemoveObserver(SceneTreeRenderObserver* observer);
  bool HasObserver(SceneTreeRenderObserver* observer) const;

  virtual void OnFrameUpdateBegin(const FrameArgs& args) {};
  virtual bool OnUpdateNode(RenderNode* node, const FrameArgs& args);
  virtual void OnFrameUpdateEnd(const FrameArgs& args) {};

  virtual void OnFrameRenderBegin(Renderer* renderer) {};
  virtual bool OnRenderNode(RenderNode* node, Renderer* renderer);
  virtual void OnFrameRenderEnd(Renderer* renderer) {};
 protected:
  void DoFrameUpdateBegin(const FrameArgs& args);
  void DoFrameUpdateEnd(const FrameArgs& args);
  void DoFrameRenderBegin(Renderer* renderer);
  void DoFrameRenderEnd(Renderer* renderer);

  void UpdateNodeRecusive(RenderNode* node, const FrameArgs& args);
  void RenderNodeRecusive(RenderNode* node, Renderer* renderer);
  RenderNodePtr root_;
  scoped_refptr<RenderTreeBuilderDelegate> delegate_;
  const Camera* camera_;
  ::base::ObserverList<SceneTreeRenderObserver> observers_;
  DISALLOW_COPY_AND_ASSIGN(SceneTreeRender);
};
}  // namespace azer
