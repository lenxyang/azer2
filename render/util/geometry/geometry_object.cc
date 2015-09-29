#include "azer/render/util/geometry/geometry_object.h"

#include "azer/math/math.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/util/effects/pvw_effect.h"

namespace azer {
GeometryObject::GeometryObject(VertexDescPtr desc)
    : desc_(desc) {
}

GeometryObject::~GeometryObject() {
}

void GeometryObject::Update(const Camera& camera) {
  world_ = std::move(holder_.GenWorldMatrix());
  pvw_ = std::move(camera.GetProjViewMatrix() * world_);
}

void GeometryObject::Render(Renderer* renderer) {
  renderer->DrawIndex(vb_, ib_, kTriangleList);
}

void GeometryObject::RenderWireframe(Renderer* renderer) {
  renderer->DrawIndex(vb_.get(), edge_ib_.get(), kLineList);
}

}  // namespace azer
