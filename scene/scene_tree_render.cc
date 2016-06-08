#include "azer/scene/scene_tree_render.h"

#include "base/logging.h"
#include "azer/scene/render_node.h"
#include "azer/scene/render_env_node.h"
#include "azer/scene/scene_node.h"

namespace azer {

SceneTreeRender::SceneTreeRender() : camera_(NULL) {}

void SceneTreeRender::SetTreeBuildDelegate(
    scoped_refptr<RenderTreeBuilderDelegate> delegate) {
  delegate_ = delegate;
}

void SceneTreeRender::Init(SceneNode* root, const Camera* camera) {
  DCHECK(delegate_.get());
  CHECK(root_ == NULL);
  camera_ = camera;
  RenderTreeBuilder builder(delegate_.get());
  root_ = builder.Build(root, camera);
}

void SceneTreeRender::Update(const FrameArgs& args) {
  root_->GetEnvNode()->UpdateRecusive(args);
  DoFrameUpdateBegin(args);
  UpdateNodeRecusive(root_.get(), args);
  DoFrameUpdateEnd(args);
}

void SceneTreeRender::Render(Renderer* renderer) {
  DoFrameRenderBegin(renderer);
  RenderNodeRecusive(root_.get(), renderer);
  DoFrameRenderEnd(renderer);
}

bool SceneTreeRender::OnUpdateNode(RenderNode* node, const FrameArgs& args) {
  if (!node->GetSceneNode()->visible()) {
    return false;
  }
  node->Update(args);
  return true;
}

bool SceneTreeRender::OnRenderNode(RenderNode* node, Renderer* renderer) {
  if (!node->GetSceneNode()->visible()) {
    return false;
  }

  node->Render(renderer);
  return true;
}

void SceneTreeRender::UpdateNodeRecusive(RenderNode* node, const FrameArgs& args) {
  if (OnUpdateNode(node, args)) {
    for (auto iter = node->children().begin(); 
         iter != node->children().end(); ++iter) {
      UpdateNodeRecusive(iter->get(), args);
    }
  }
}

void SceneTreeRender::RenderNodeRecusive(RenderNode* node, Renderer* renderer) {
  if (OnRenderNode(node, renderer)) {
    for (auto iter = node->children().begin(); 
         iter != node->children().end(); ++iter) {
      RenderNodeRecusive(iter->get(), renderer);
    }
  }
}

void SceneTreeRender::AddObserver(SceneTreeRenderObserver* observer) {
  observers_.AddObserver(observer);
}

void SceneTreeRender::RemoveObserver(SceneTreeRenderObserver* observer) {
  observers_.RemoveObserver(observer);
}

bool SceneTreeRender::HasObserver(SceneTreeRenderObserver* observer) const {
  return observers_.HasObserver(observer);
}

void SceneTreeRender::DoFrameUpdateBegin(const FrameArgs& args) {
  OnFrameUpdateBegin(args);
  FOR_EACH_OBSERVER(SceneTreeRenderObserver, 
                    observers_, 
                    OnFrameUpdateBegin(this, args));
}

void SceneTreeRender::DoFrameUpdateEnd(const FrameArgs& args) {
  OnFrameUpdateEnd(args);
  FOR_EACH_OBSERVER(SceneTreeRenderObserver, 
                    observers_, 
                    OnFrameUpdateEnd(this, args));
}

void SceneTreeRender::DoFrameRenderBegin(Renderer* renderer) {
  OnFrameRenderBegin(renderer);
  FOR_EACH_OBSERVER(SceneTreeRenderObserver, 
                    observers_, 
                    OnFrameRenderBegin(this, renderer));
}

void SceneTreeRender::DoFrameRenderEnd(Renderer* renderer) {
  OnFrameRenderEnd(renderer);
  FOR_EACH_OBSERVER(SceneTreeRenderObserver, 
                    observers_, 
                    OnFrameRenderEnd(this, renderer));
}
}  // namespace azer
