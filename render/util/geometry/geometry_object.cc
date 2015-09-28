#include "azer/render/util/geometry/geometry_object.h"

#include "azer/math/math.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/util/pvw_effect.h"

namespace azer {
GeometryObject::GeometryObject(PVWEffectPtr effect)
    : effect_(effect) {
}

GeometryObject::~GeometryObject() {
}

void GeometryObject::Update(const Camera& camera) {
  world_ = std::move(holder_.GenWorldMatrix());
  pvw_ = std::move(camera.GetProjViewMatrix() * world_);
}

void GeometryObject::Render(Renderer* renderer) {
  effect_->SetWorld(world_);
  effect_->SetPVW(pvw_);
  effect_->Use(renderer);
  renderer->DrawIndex(vb_, ib_, kTriangleList);
}

void GeometryObject::RenderWireframe(Renderer* renderer) {
  effect_->SetWorld(world_);
  effect_->SetPVW(pvw_);
  effect_->Use(renderer);
  renderer->DrawIndex(vb_.get(), edge_ib_.get(), kLineList);
}
}  // namespace azer
