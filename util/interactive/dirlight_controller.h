#pragma once


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
class ArrowObject;
class ColorEffect;
class InteractiveContext;
class TranslateController;
class RotateController;
class Renderer;

class DirLightObject : public ::base::RefCounted<DirLightObject> {
 public:
  explicit DirLightObject(Light* light);

  Light* light() { return light_.get();}
  void set_position(const Vector3& pos) { position_ = pos;}
  const Vector3& position() const { return position_;}
  void Render(const Camera& camera, Renderer* renderer);
 private:
  Vector3 position_;
  LightPtr light_;
  scoped_ptr<ArrowObject> object_;
  DISALLOW_COPY_AND_ASSIGN(DirLightObject);
};

class DirLightController : public InteractiveController,
                           public RotateControllerObserver {
 public:
  explicit DirLightController(InteractiveContext* ctx);
  ~DirLightController() override;

  void SetDirLightObj(DirLightObject* obj);
  Light* light() { return lightobj()->light();}
  DirLightObject* lightobj() { return lightobj_.get();}
  void OnActive() override;
  void OnDeactive() override;
  int32_t GetPicking(const gfx::Point& pt) override;
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
  bool dragging_;
  scoped_refptr<DirLightObject> lightobj_;
  scoped_ptr<RotateController> controller_;
  DISALLOW_COPY_AND_ASSIGN(DirLightController);
};
}  // namespace azer
