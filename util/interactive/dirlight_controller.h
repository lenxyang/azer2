#pragma once

#include "base/basictypes.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/light.h"
#include "azer/math/quaternion.h" 
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 
#include "azer/render/entity.h"
#include "azer/util/interactive/interactive_controller.h"
#include "azer/util/interactive/translate_controller.h"
#include "azer/util/interactive/rotate_controller.h"

namespace azer {
class ColorEffect;
class InteractiveContext;
class TranslateController;
class RotateController;
class Renderer;

class DirLightControllerObj {
 public:
  DirLightControllerObj();

  const Vector3& position() const { return position_;}
  void set_scale(const Vector3& v);
  void set_position(const Vector3& pos);
  void set_orientation(const Quaternion& q);
  void set_color(const Vector4& c);
  void Update(const Camera* camera);
  void Render(Renderer* renderer);
 private:
  void OnParamUpdate();
  EntityPtr arrow_;
  Vector4 color_;
  Vector3 scale_;
  Vector3 position_;
  Quaternion orientation_;
  scoped_refptr<ColorEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(DirLightControllerObj);
};

class DirLightController : public InteractiveController,
                           public RotateControllerObserver {
 public:
  DirLightController(Light* light, InteractiveContext* ctx);
  ~DirLightController() override;

  void set_position(const Vector3& pos);
  const Vector3& position() const;
  void show_rotate_controller(bool b);

  void OnActive() override;
  void OnDeactive() override;
  int32 GetPicking(const gfx::Point& pt) override;
  void OnDragBegin(const ui::MouseEvent& e) override;
  void OnDragging(const ui::MouseEvent& e) override;
  void OnDragEnd(const ui::MouseEvent& e) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  // override from DirLightController
  void OnRotateBegin(RotateController* controller) override;
  void OnRotating(RotateController* controller) override;
  void OnRotateEnd(RotateController* controller) override;
 private:
  void UpdateParam();
  bool show_rotate_controller_;
  bool dragging_;
  LightPtr light_;

  scoped_ptr<RotateController> controller_;
  scoped_ptr<DirLightControllerObj> object_;
  DISALLOW_COPY_AND_ASSIGN(DirLightController);
};
}  // namespace azer
