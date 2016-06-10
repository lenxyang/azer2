#pragma once


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
class Light;
typedef scoped_refptr<Blending> BlendingPtr;

class SpotLightObject : public ::base::RefCounted<SpotLightObject> {
 public:
  explicit SpotLightObject(Light* light);
  ~SpotLightObject();

  void set_scale(const Vector3& s) { scale_ = s;}
  const Vector3& scale() const { return scale_;}
  Light* light() { return light_.get();}
  void Render(const Camera& camera, Renderer* renderer);
 private:
  LightPtr light_;
  EntityPtr entity_;
  Vector3 scale_;
  scoped_refptr<ColorEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(SpotLightObject);
};

class SpotLightDirectionalObject {
 public:
  explicit SpotLightDirectionalObject(Light* light);
  ~SpotLightDirectionalObject();

  void ResetColor();
  void Update(const Camera& camera);
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
  EntityPtr dirline_;
  BlendingPtr blending_;
  float theta_;
  float phi_;
  float range_;
  Vector4 inner_color_;
  Vector4 outer_color_;
  Vector3 position_;
  Quaternion orientation_;
  Matrix4 pv_;
  Matrix4 world_;
  DISALLOW_COPY_AND_ASSIGN(SpotLightDirectionalObject);
};

class SpotLightController;
class SpotLightControllerObserver {
 public:
  virtual void OnSpotLightPositionChanged(SpotLightController* controller) {}
  virtual void OnSpotLightOrientationChanged(SpotLightController* controller) {}
};

class SpotLightController : public InteractiveController,
                            public RotateControllerObserver,
                            public TranslateControllerObserver {
 public:
  explicit SpotLightController(InteractiveContext* ctx);
  ~SpotLightController();

  enum {
    kNone = 0,
    kTranslate,
    kRotate,
  };
  
  void SetLight(Light* ptr);
  void set_mode(int32_t mode);
  const Vector3& scale() const { return scale_;}
  void set_scale(const Vector3& v) { scale_ = v;}

  void OnActive() override;
  void OnDeactive() override;
  int32_t GetPicking(const gfx::Point& pt) override;
  void OnDragBegin(const ui::MouseEvent& e) override;
  void OnDragging(const ui::MouseEvent& e) override;
  void OnDragEnd(const ui::MouseEvent& e) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  // override form RotateControllerObserver
  void OnRotateBegin(RotateController* controller) override;
  void OnRotating(RotateController* controller) override;
  void OnRotateEnd(RotateController* controller) override;

  // override from TranslateControllerObserver
  void OnTranslateBegin(TranslateController* controller) override;
  void OnTranslating(TranslateController* controller) override;
  void OnTranslateEnd(TranslateController* controller) override;

  void AddSpotLightObserver(SpotLightControllerObserver* obs);
  void RemoveSpotLightObserver(SpotLightControllerObserver* obs);
  bool HasSpotLightObserver(SpotLightControllerObserver* obs);
 private:
  LightPtr light_;
  int32_t mode_;
  int32_t new_mode_;
  Vector3 scale_;
  std::unique_ptr<TranslateController> translate_controller_;
  std::unique_ptr<RotateController> rotate_controller_;
  std::unique_ptr<SpotLightDirectionalObject> object_;
  ::base::ObserverList<SpotLightControllerObserver> observer_list_;
  DISALLOW_COPY_AND_ASSIGN(SpotLightController);
};
}  // namespace azer
