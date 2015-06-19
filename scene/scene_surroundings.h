#pragma once

#include <deque>

#include "azer/base/config_node.h"
#include "azer/render/light.h"
#include "azer/render/sky.h"
#include "azer/render/camera.h"

namespace azer {
/**
 * SceneSurroundings
 * 一般来说 EffectParamsProvider 的内容分成两个部分，一部分是自包含的，
 * 即仅仅通过自身就可以获得（有可能是通过与自身绑定的 SceneNode），
 * 另外一部分是外部信息，如光照，阴影深度等
 * SceneSurroundings 正是保存了环境相关的内容，他的信息一般通过 Push Pop 两个
 * 结对操作设置，例如某个节点绑定了光源，那么所有这个节点的子节点都会收到光源的
 * 影响，在子节点开始渲染时，将光源的信息压栈；在所有子节点渲染完成以后，将光源
 * 信息出栈。
 *
 */
class SceneSurroundings {
 public:
  SceneSurroundings();
  void SetCamera(const Camera* camera);
  void PushLight(LightPtr);
  void PopLight();
  void PushConfig(ConfigNodePtr node);
  void PopConfig();
  void reset();

  void PushSky(SkyPtr sky);
  void PopSky();
  
  const Camera* camera() const { return camera_;}
  const std::deque<LightPtr>& lights() const { return lights_;};
  const std::deque<ConfigNodePtr>& configs() const { return configs_;}
 private:
  std::deque<LightPtr> lights_;
  std::deque<ConfigNodePtr> configs_;
  std::defque<SkyPtr> sky_;
  const Camera* camera_;
  DISALLOW_COPY_AND_ASSIGN(SceneSurroundings);
};
}  // namespace azer
