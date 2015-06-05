#include "studio/scene/scene_render.h"

#include "azer/render/effect_params_provider.h"
#include "azer/render/effect.h"
#include "azer/render/renderer.h"
#include "azer/render/renderable_object.h"

#include "studio/scene/scene_environment.h"
#include "studio/scene/scene_node_data.h"

namespace azer {

// SceneRender
SceneRender::SceneRender(SceneNodePtr node) 
    : node_(node) {
  env_.reset(new SceneEnvironment);
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
  env_->reset();
  OnUpdateBegin();
  FOR_EACH_OBSERVER(SceneRenderObserver,
                    observers_,
                    OnSceneUpdateBegin(this));
  UpdateSceneRecusive(env_.get(), node_);
  FOR_EACH_OBSERVER(SceneRenderObserver,
                    observers_,
                    OnSceneUpdateEnd(this));
  OnUpdateEnd();
}

void SceneRender::UpdateScene(SceneEnvironment* env, SceneNode* node) {
  RenderableObjectPtr obj = node->mutable_data()->GetRenderableObject();
  DCHECK(obj.get());
  SceneEffectParamsProvider* provider = (SceneEffectParamsProvider*)obj->provider();
  provider->SetSceneNode(node);
  provider->SetSceneEnvironment(env);
  provider->CalcParams();
  PrepareRender(node);
}

void SceneRender::UpdateSceneRecusive(SceneEnvironment* env, SceneNode* node) {
  if (node->data().type() == SceneNodeData::kRenderableObject) {
    UpdateScene(env, node);
  } else if (node->data().type() == SceneNodeData::kLight) {
    env->PushLight(node->mutable_data()->GetLight());
  }

  for (auto iter = node->children().begin(); 
       iter != node->children().end(); ++iter) {
    UpdateSceneRecusive(env, *iter);
  }

  if (node->data().type() == SceneNodeData::kLight) {
    env->PopLight();
  }
}

// class SimpleSceneRender
SimpleSceneRender::SimpleSceneRender(SceneNodePtr node) 
    : SceneRender(node) {
}

void SimpleSceneRender::PrepareRender(SceneNode* node) {
  DCHECK(node->data().type() == SceneNodeData::kRenderableObject);
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
