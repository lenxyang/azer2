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

MeshPartPtr GeometryObject::CreateObject(Effect* effect) {
  EntityPtr entity(new Entity(vb_, ib_));
  MeshPartPtr part(new MeshPart(effect));
  part->AddEntity(entity);
  return part;
}

MeshPartPtr GeometryObject::CreateFrameObject(Effect* effect) {
  EntityPtr entity(new Entity(vb_, frame_ib_));
  MeshPartPtr part(new MeshPart(effect));
  part->AddEntity(entity);
  return part;
}

void GeometryObject::Render(Renderer* renderer) {
  renderer->UseVertexBuffer(vb_.get());
  renderer->UseIndicesBuffer(ib_.get());
  renderer->SetPrimitiveTopology(kTriangleList);
  renderer->DrawIndex(ib_->indices_num(), 0, 0);
}

void GeometryObject::RenderWireframe(Renderer* renderer) {
  renderer->UseVertexBuffer(vb_.get());
  renderer->UseIndicesBuffer(frame_ib_.get());
  renderer->SetPrimitiveTopology(kLineList);
  renderer->DrawIndex(frame_ib_->indices_num(), 0, 0);
}

}  // namespace azer
