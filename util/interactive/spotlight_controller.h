#pragma once

#include "base/basictypes.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/light.h"
#include "azer/math/quaternion.h" 
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 
#include "azer/render/entity.h"
#include "azer/util/interactive/interactive_controller.h"

namespace azer {

class Blending;
class ColorEffect;
typedef scoped_refptr<Blending> BlendingPtr;

class SpotLightControllerObj {
 public:
  SpotLightControllerObj();

  void ResetColor();
  void Update(const Camera* camera, const Vector3& position);
  void Render(Renderer* renderer);

  void set_theta(float v) { theta_ = v;}
  void set_phi(float v) { phi_ = v;}
  void set_range(float v) { range_ = v;}
  void set_inner_color(const Vector4& v} { inner_color_ = v;}
  void set_outer_color(const Vector4& v} { outer_color_ = v;}
 private:
  void InitCircle();
  void InitBarrel();
  RasterizerStatePtr rasterizer_state_;
  scoped_refptr<ColorEffect> color_effect_;
  EntityPtr inner_cylinder_;
  EntityPtr outer_cylinder_;
  EntityPtr lightobj_;
  BlendingPtr blending_;
  float theta_;
  float phi_;
  float range_;
  Vector4 inner_color_;
  Vector4 outer_color_;
  DISALLOW_COPY_AND_ASSIGN(SpotLightControllerObj);
};

class SpotLightController;
class SpotLightControllerObserver {
 public:
  virtual void OnSpotLightPositionChanged(SpotLightController* controller) {}
  virtual void OnSpotLightOrientationChanged(SpotLightController* controller) {}
};

class SpotLightController : public InteractiveController {
 public:
  explicit SpotLightController(InteractiveContext* ctx);
  ~SpotLightController();

  void SetLight(Light* ptr);

  int32 GetPicking(const gfx::Point& pt) override;
  void OnDragBegin(const gfx::Point& pt) override;
  void OnDrag(const gfx::Point& pt) override;
  void OnDragEnd(const gfx::Point& pt) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  void AddSpotLightObserver(SpotLightControllerObserver* observer);
  void RemoteSpotLightObserver(SpotLightControllerObserver* observer);
  bool HasSpotLightObserver(SpotLightControllerObserver* observer);
 private:
  LightPtr light_;
  ObserverList<SpotLightControllerObserver> observer_list_;
  DISALLOW_COPY_AND_ASSIGN(SpotLightController);
};
}  // namespace azer
