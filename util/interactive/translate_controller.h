#pragma once

#include <memory>

#include "azer/math/plane.h" 
#include "azer/math/vector3.h"
#include "azer/math/vector4.h"
#include "azer/math/matrix4.h"
#include "azer/render/entity.h"
#include "azer/util/interactive/interactive_controller.h"

namespace azer {
class AmbientColorEffect;
class Camera;
class Renderer;
class ColorEffect;
class Plane;

class TranslateControlObj {
 public:
  enum {
    kAxisX = 0,
    kAxisY,
    kAxisZ,
    kLineXY,
    kLineXZ,
    kLineYZ,
    kLineYX,
    kLineZX,
    kLineZY,
    kPlaneXY,
    kPlaneYZ,
    kPlaneZX,
    kSubsetCount,
  };

  TranslateControlObj();
  void SetScale(const Vector3& v) { scale_ = v;}
  void SetColor(int32_t index, const Vector4& color) { colors_[index] = color;}
  void ResetColor();
  void Update(const Camera& camera, const Vector3& position);
  void Render(Renderer* renderer);

  float plane_width() const;
  float axis_begin() const;
 private:
  EntityDataPtr InitAxesObjects(VertexDesc* desc);
  EntityDataPtr InitAxesConeData(VertexDesc* desc);
  Vector3 scale_;
  Vector4 colors_[kSubsetCount];
  Matrix4 world_;
  Matrix4 pv_;
  EntityPtr entity_;
  EntityPtr cones_;
  scoped_refptr<AmbientColorEffect> ambient_effect_;
  scoped_refptr<ColorEffect> color_effect_;
  static const float kPlaneWidth;
  static const float kAxisBegin;
  DISALLOW_COPY_AND_ASSIGN(TranslateControlObj);
};

class TranslateController;
class TranslateControllerObserver {
 public:
  virtual void OnTranslateBegin(TranslateController* controller) {}
  virtual void OnTranslating(TranslateController* controller) {}
  virtual void OnTranslateEnd(TranslateController* controller) {}
};

class TranslateController : public InteractiveController {
 public:
  explicit TranslateController(InteractiveContext* ctx);
  ~TranslateController() override;

  enum {
    kHitNone = 0,
    kHitAxisX,
    kHitAxisY,
    kHitAxisZ,
    kHitPlaneXY,
    kHitPlaneYZ,
    kHitPlaneZX,
  };

  int32_t GetPicking(const gfx::Point& pt) override;
  void OnDragBegin(const ui::MouseEvent& e) override;
  void OnDragging(const ui::MouseEvent& e) override;
  void OnDragEnd(const ui::MouseEvent& e) override;
  void UpdateFrame(const FrameArgs& args) override;
  void RenderFrame(Renderer* renderer) override;

  void set_position(const Vector3& pos) { position_ = pos;}
  const Vector3& position() const { return position_;}
  void set_scale(const Vector3& s) { scale_ = s;}
  const Vector3& scale() const { return scale_;}
  
  void AddTranslateObserver(TranslateControllerObserver* observer);
  void RemoveTranslateObserver(TranslateControllerObserver* observer);
  bool HasTranslateObserver(TranslateControllerObserver* observer);
 private:
  /*
   * 拖动时的起始点
   */
  void GetDragInitPos(const Ray& ray, Vector3* pos);
  void CalcDragOffset(const Ray& ray, Vector3* offset);
  Vector3 position_;
  Vector3 origin_position_;
  Vector3 draginit_pos_;
  Vector3 scale_;
  std::unique_ptr<TranslateControlObj> object_;
  Plane plane_[3];
  ObserverList<TranslateControllerObserver> observer_list_;
  static const Vector4 kSelectedColor;
  static const Vector4 kSelectedPlaneColor;
  DISALLOW_COPY_AND_ASSIGN(TranslateController);
};

}  // namespace azer
