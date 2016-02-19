#pragma once

namespace azer {

class Light;
class LightObserver {
 public:
  virtual void OnLightPositionChanged(Light* light) {}
  virtual void OnLightDirectionChanged(Light* light) {}
  virtual void OnLightPropertyLight(Light* light) {}
};

}  // namespace azer
