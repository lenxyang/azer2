#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "azer/render/render.h"

namespace azer {
class RenderTreeBuilderDelegate;
class RenderNode;
class RenderEnvNode;
class SceneNode;
class SceneRender;
typedef scoped_refptr<RenderNode> RenderNodePtr;

class SceneRenderObserver {
 public:
  virtual void OnFrameUpdateBegin(SceneRender* sr, const FrameArgs& args) {}
  virtual void OnFrameUpdateEnd(SceneRender* sr, const FrameArgs& args) {}
  virtual void OnFrameRenderBegin(SceneRender* sr, Renderer* renderer) {}
  virtual void OnFrameRenderEnd(SceneRender* sr, Renderer* renderer) {}
};

class SceneRender {
 public:
  SceneRender();

  RenderNode* root() { return root_.get();}
  const Camera* camera() const { return camera_;}
  void SetTreeBuildDelegate(scoped_ptr<RenderTreeBuilderDelegate> delegate);
  void Init(SceneNode* root, const Camera* camera);
  void Update(const FrameArgs& args);
  void Render(Renderer* renderer);

  void AddObserver(SceneRenderObserver* observer);
  void RemoveObserver(SceneRenderObserver* observer);
  bool HasObserver(SceneRenderObserver* observer) const;

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
  scoped_ptr<RenderTreeBuilderDelegate> delegate_;
  const Camera* camera_;
  ObserverList<SceneRenderObserver> observers_;
  DISALLOW_COPY_AND_ASSIGN(SceneRender);
};
}  // namespace azer
