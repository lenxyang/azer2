#pragma once

#include "azer/base/config_node.h"
#include "azer/render/effect_params_provider.h"
#include "azer/render/renderable_object.h"
#include "azer/render/light.h"

#include "azer/scene/scene_effect_params_provider.h"

namespace azer {
class SceneNodeData {
 public:
  SceneNodeData();
  
  void Attach(LightPtr light);
  void Attach(RenderableObjectPtr object);
  void Attach(ConfigNodePtr surrounding);
  void DetachAll();
  
  RenderableObjectPtr& GetRenderableObject() { return renderable_;}
  const RenderableObjectPtr& GetRenderableObject() const { return renderable_;}

  ConfigNodePtr& GetSurroundings() { return surroundings_;}
  const ConfigNodePtr& GetSurroundings() const { return surroundings_;}

  LightPtr& GetLight() { return light_;}
  const LightPtr& GetLight() const { return light_;}
 private:
  RenderableObjectPtr renderable_;
  LightPtr light_;
  ConfigNodePtr surroundings_;
  DISALLOW_COPY_AND_ASSIGN(SceneNodeData);
};
}  // namespace azer
