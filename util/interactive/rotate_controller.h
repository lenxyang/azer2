#pragma once

#include "base/basictypes.h"
#include "azer/effect/color_effect.h"
#include "azer/math/quaternion.h" 
#include "azer/math/vector3.h"
#include "azer/math/vector4.h" 
#include "azer/render/entity.h"
#include "azer/util/interactive/interactive_controller.h"

namespace azer {
class EntityData;
class Camera;
class RotateController;
class InteractiveContext;

class RotateControllerObj {
 public:
  enum {
    kAxisX = 0,
    kAxisY,
    kAxisZ,
    kSphere,
    kRoundX,
    kRoundY,
    kRoundZ,
    kSubsetCount,
  };
  RotateControllerObj();

  float circle_radius() const;
  void SetScale(const Vector3& s) { scale_ = s;}
  const Vector3& scale() const { return scale_;}
  void SetColor(int32 index, const Vector4& c);
  void ResetColor();
  void Update(const Camera* camera, const Vector3& position);
  void Render(Renderer* renderer);
 private:
  void InitEntity();
  void AppendAxisData(EntityData* data);
  void AppendRoundData(EntityData* data);
  Vector4 colors_[kSubsetCount];
  EntityPtr entity_;
  Vector3 scale_;
  scoped_refptr<AmbientColorEffect> ambient_effect_;
  scoped_refptr<ColorEffect> color_effect_;
  static const float CirclekMargin;
  static const float kAxisLength; 
  DISALLOW_COPY_AND_ASSIGN(RotateControllerObj);
};

class RotateControllerObserver {
 public:
  virtual void OnRotateBegin(RotateController* controller) {}
  virtual void OnRotating(RotateController* controller) {}
  virtual void OnRotateEnd(RotateController* controller) {}
};

class RotateController : public InteractiveController {
 public:
  enum {
    kHitNone,
    kHitAxisX,
    kHitAxisY,
    kHitAxisZ,
  };
  explicit RotateController(InteractiveContext* ctx);
  ~RotateController() override;

  int32 GetPicking(const gfx::Point& pt) override;
  void OnDragBegin(const gfx::Point& pt) override;
  void OnDrag(const gfx::Point& pt) override;
  void OnDragEnd(const gfx::Point& pt) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  const Quaternion& orientation() const { return orientation_;}
  void set_position(const Vector3& pos) { position_ = pos;}
  const Vector3& position() const { return position_;}
  void set_scale(const Vector3& s) { scale_ = s;}
  const Vector3& scale() const { return scale_;}

  void AddRotateObserver(RotateControllerObserver* observer);
  void RemoveRotateObserver(RotateControllerObserver* observer);
  bool HasRotateObserver(RotateControllerObserver* observer);
 private:
  const Quaternion orientation_;
  Vector3 scale_;
  Vector3 position_;
  scoped_ptr<RotateControllerObj> object_;
  ObserverList<RotateControllerObserver> observer_list_;
  static const Vector4 kSelectedColor;
  DISALLOW_COPY_AND_ASSIGN(RotateController);
};

}  // namespace azer
