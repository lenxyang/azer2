#pragma once

#include "base/basictypes.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/light.h"
#include "azer/math/quaternion.h" 
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 
#include "azer/render/entity.h"
#include "azer/render/rasterizer_state.h"
#include "azer/util/interactive/rotate_controller.h"
#include "azer/util/interactive/translate_controller.h"
#include "azer/util/interactive/interactive_controller.h"

namespace azer {
class Blending;
class Camera;
class ColorEffect;
typedef scoped_refptr<Blending> BlendingPtr;

class SpotLightObject : public ::base::RefCounted<SpotLightObject> {
 public:
  SpotLightObject();
  ~SpotLightObject();
 private:
  DISALLOW_COPY_AND_ASSIGN(SpotLightObject);
};

class SpotLightControllerObject {
 public:
  explicit SpotLightControllerObject(Light* light);
  ~SpotLightControllerObject();

  void ResetColor();
  void Update(const Camera* camera);
  void Render(Renderer* renderer);

  void set_position(const Vector3& pos) { position_ = pos;}
  void set_orientation(const Quaternion& orientation) { orientation_ = orientation;}
  void set_inner_color(const Vector4& v) { inner_color_ = v;}
  void set_outer_color(const Vector4& v) { outer_color_ = v;}
 private:
  void InitCircle();
  void InitBarrel();
  RasterizerStatePtr rasterizer_state_;
  scoped_refptr<ColorEffect> color_effect_;
  EntityPtr inner_object_;
  EntityPtr outer_object_;
  EntityPtr lightobj_;
  BlendingPtr blending_;
  float theta_;
  float phi_;
  float range_;
  Vector4 inner_color_;
  Vector4 outer_color_;
  Vector3 position_;
  Quaternion orientation_;
  DISALLOW_COPY_AND_ASSIGN(SpotLightControllerObject);
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
  void OnDragBegin(const ui::MouseEvent& e) override;
  void OnDragging(const ui::MouseEvent& e) override;
  void OnDragEnd(const ui::MouseEvent& e) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;
 private:
  LightPtr light_;
  scoped_ptr<TranslateController> translate_controller_;
  scoped_ptr<RotateController> rotate_controller_;
  scoped_ptr<SpotLightControllerObject> object_;
  ObserverList<SpotLightControllerObserver> observer_list_;
  DISALLOW_COPY_AND_ASSIGN(SpotLightController);
};
}  // namespace azer
