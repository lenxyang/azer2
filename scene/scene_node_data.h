#pragma once

#include "azer/base/config_node.h"
#include "azer/render/effect_params_provider.h"
#include "azer/render/renderable_object.h"
#include "azer/render/light.h"
#include "azer/render/sky.h"

#include "azer/scene/scene_effect_params_provider.h"

namespace azer {
class SceneNodeData {
 public:
  SceneNodeData();
  
  void AttachLight(LightPtr light);
  void AttachRenderable(RenderableObjectPtr object);
  void AttachSky(SkyPtr object);
  void AttachConfig(ConfigNodePtr surrounding);
  void DetachAll();
  
  RenderableObjectPtr& GetRenderableObject() { return renderable_;}
  const RenderableObjectPtr& GetRenderableObject() const { return renderable_;}

  ConfigNodePtr& GetSurroundings() { return surroundings_;}
  const ConfigNodePtr& GetSurroundings() const { return surroundings_;}

  LightPtr& GetLight() { return light_;}
  const LightPtr& GetLight() const { return light_;}

  SkyPtr& GetSky() { return sky_;}
  const SkyPtr& GetSky() const { return sky_;}
 private:
  RenderableObjectPtr renderable_;
  LightPtr light_;
  ConfigNodePtr surroundings_;
  SkyPtr sky_;
  DISALLOW_COPY_AND_ASSIGN(SceneNodeData);
};
}  // namespace azer
