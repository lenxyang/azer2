#pragma once

#include "azer/render/effect_params_provider.h"
#include "azer/render/renderable_object.h"
#include "azer/render/light.h"

#include "azer/scene/scene_effect_params_provider.h"

namespace azer {
class SceneNodeData {
 public:
  enum {
    kNotSpecified,
    kLight,
    kRenderableObject,
  };

  SceneNodeData();
  
  int type() const { return type_;}

  void Attach(LightPtr light);
  void Attach(RenderableObjectPtr object);
  void Detach();
  
  RenderableObjectPtr& GetRenderableObject() { return renderable_;}
  const RenderableObjectPtr& GetRenderableObject() const { return renderable_;}

  LightPtr& GetLight() { return light_;}
  const LightPtr& GetLight() const { return light_;}

  bool attached() const { return type() != kNotSpecified;}
 private:
  int type_;
  RenderableObjectPtr renderable_;
  LightPtr light_;
  DISALLOW_COPY_AND_ASSIGN(SceneNodeData);
};
}  // namespace azer
