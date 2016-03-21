#include "azer/util/interactive/rotate_controller.h"

#include "base/logging.h"
#include "azer/math/plane.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_pack.h"
#include "azer/util/interactive/env.h"
#include "azer/util/geometry/geometry.h"
#include "azer/util/interactive/interactive_context.h"
#include "azer/util/interactive/pick_util.h"

namespace azer {
// class RotateControlerObj
const float RotateControlerObj::kAxisLength = 0.6f;; 
RotateControlerObj::RotateControlerObj() {
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
}

void RotateControlerObj::InitEntity() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  ambient_effect_ = (AmbientColorEffect*)env->GetEffect("AmbientColorEffect");
  color_effect_ = (ColorEffect*)env->GetEffect("ColorEffect");

  VertexDataPtr vdata(new VertexData(ambient_effect_->vertex_desc(), 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata, idata));
  AppendAxisData(data);
}

void RotateControlerObj::AppendRoundData(EntityData* data) {
  GeoSphereParam sphere_param;
  AppendGeoSphereData(data, sphere_param);
  AppendGeoRoundData(data, sphere_param.radius, sphere_param.radius);
  AppendGeoRoundData(data, sphere_param.radius, sphere_param.radius);
  AppendGeoRoundData(data, sphere_param.radius, sphere_param.radius);
}

void RotateControlerObj::AppendAxisData(EntityData* data) {
  const int32 kVertexCount = 6;
  data->vdata()->extend(kVertexCount);
  int32 base = data->vdata()->vertex_count() - kVertexCount;
  VertexPack vpack(data->vdata());
  vpack.move(base);
  VertexPos vpos(0, 0);
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kAxisLength, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kAxisLength, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kAxisLength, 1.0f), vpos);
  CHECK(vpack.next(1));
  data->AddSubset(Subset(base, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 2, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 4, 2, 0, 0, kLineList));
}

void RotateControlerObj::SetColor(int32 index, const Vector4& c) {
  DCHECK_LT(index, static_cast<int32>(arraysize(colors_)));
  colors_[index] = c;
}

void RotateControlerObj::ResetColor() {
  colors_[kAxisX] = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
  colors_[kAxisY] = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
  colors_[kAxisZ] = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
  colors_[kRoundX] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  colors_[kRoundY] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  colors_[kRoundZ] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  colors_[kSphere] = Vector4(0.8.0f, 0.8f, 0.8f, 0.4f);
}

// class RotateController
RotateController::RotateController(InteractiveContext* ctx) 
    : InteractiveController(ctx) {
}

RotateController::~RotateController() {
}

int32 RotateController::GetPicking(const gfx::Point& pt) {
  return 0;
}

void RotateController::UpdateFrame(const FrameArgs& args) {
}

void RotateController::RenderFrame(Renderer* renderer) {
}
}  // namespace azer
