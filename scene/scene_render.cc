#include "azer/scene/scene_render.h"

#include "azer/render/effect_params_provider.h"
#include "azer/render/effect.h"
#include "azer/render/renderer.h"
#include "azer/render/renderable_object.h"

#include "azer/scene/scene_surroundings.h"
#include "azer/scene/scene_node_data.h"

namespace azer {

// SceneRender
SceneRender::SceneRender(SceneNodePtr node) 
    : node_(node) {
  surroundings_.reset(new SceneSurroundings);
}

SceneRender::~SceneRender() {
}

void SceneRender::AddObserver(SceneRenderObserver* observer) {
  observers_.AddObserver(observer);
}

void SceneRender::RemoveObserver(SceneRenderObserver* observer) {
  observers_.RemoveObserver(observer);
}

bool SceneRender::HasObserver(SceneRenderObserver* observer) {
  return observers_.HasObserver(observer);
}

void SceneRender::Update(const FrameArgs& frame) {
  frame_ = &frame;
  surroundings_->reset();
  OnUpdateBegin();
  FOR_EACH_OBSERVER(SceneRenderObserver,
                    observers_,
                    OnSceneUpdateBegin(this));
  UpdateSceneRecusive(surroundings_.get(), node_);
  FOR_EACH_OBSERVER(SceneRenderObserver,
                    observers_,
                    OnSceneUpdateEnd(this));
  OnUpdateEnd();
}

void SceneRender::RenderObj(RenderableObjectPtr obj, 
                            SceneSurroundings* surroundings, SceneNode* node) {
  DCHECK(obj.get());
  SceneEffectParamsProvider* provider = (SceneEffectParamsProvider*)obj->provider();
  provider->SetSceneNode(node);
  provider->SetSceneSurroundings(surroundings);
  provider->CalcParams();
  PrepareRender(node);
}

void SceneRender::UpdateSceneRecusive(SceneSurroundings* surroundings,
                                      SceneNode* node) {
  if (node->data().GetRenderableObject().get()) {
    RenderObj(node->data().GetRenderableObject(), surroundings, node);
  } 
  if (node->data().GetSky().get()) {
    RenderObj(node->data().GetSky(), surroundings, node);
  } 
  if (node->data().GetSurroundings().get()) {
    surroundings->PushConfig(node->mutable_data()->GetSurroundings());
  } 
  if (node->data().GetLight().get()) {
    surroundings->PushLight(node->mutable_data()->GetLight());
  }

  if (node->data().GetSky().get()) {
    surroundings->PushSky(node->mutable_data()->GetSky());
  }

  for (auto iter = node->children().begin(); 
       iter != node->children().end(); ++iter) {
    UpdateSceneRecusive(surroundings, *iter);
  }

  if (node->data().GetSurroundings().get()) {
    surroundings->PopConfig();
  } 
  if (node->data().GetLight().get()) {
    surroundings->PopLight();
  }
  if (node->data().GetSky().get()) {
    surroundings->PopSky();
  }
}

// class SimpleSceneRender
SimpleSceneRender::SimpleSceneRender(SceneNodePtr node) 
    : SceneRender(node) {
}

void SimpleSceneRender::PrepareRender(SceneNode* node) {
  DCHECK(node->data().GetRenderableObject().get());
  vec_.push_back(node->mutable_data()->GetRenderableObject());
}

void SimpleSceneRender::Render(int target, Renderer* renderer) {
  for (auto iter = vec_.begin(); iter != vec_.end(); ++iter) {
    RenderableObjectPtr obj = *iter;
    obj->Render(target, renderer);
  }
}

void SimpleSceneRender::OnUpdateBegin() {
  vec_.clear();
}
}  // namespace azer
