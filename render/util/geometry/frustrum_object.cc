#include "azer/render/util/geometry/frustrum_object.h"

#include "base/logging.h"
#include "azer/render/render.h"
#include "azer/render/util/vertex_pack.h"
#include "azer/render/util/geometry/box_object.h"

namespace azer {
FrustrumObject::FrustrumObject(const Camera* camera, VertexDescPtr desc)
    : GeometryObject(desc) {
  Matrix4 viewins = camera->GetViewMatrix().Inverse();
  SlotVertexDataPtr vdata = CreateBoxVertexData(desc.get());
  VertexPack vpack(vdata.get());
  VertexPos posidx;
  CHECK(GetSemanticIndex("position", 0, desc_.get(), &posidx));
  Vector4 value;
  vpack.first();
  while (!vpack.end()) {
    vpack.ReadVector4(&value, posidx);
    value.x *= 2.0f;
    value.y *= 2.0f;
    value.z += 0.5f;

    value = viewins * value;
    vpack.WriteVector4(value, posidx);
    vpack.next(1);
  }

  VertexPos normidx;
  if (GetSemanticIndex("normal", 0, desc_.get(), &normidx)) {
    vpack.first();
    Vector4 n1, n2, n3;
    while (!vpack.end()) {
      vpack.ReadVector4(&n1, normidx);
      vpack.next(1);
      vpack.ReadVector4(&n2, normidx);
      vpack.next(1);
      vpack.ReadVector4(&n3, normidx);
      vpack.next(1);

      Vector4 n = Vector4(CalcPlaneNormal(n1, n2, n3), 0.0f);
      vpack.next(-3);
      vpack.WriteVector4(n, normidx); vpack.next(1);
      vpack.WriteVector4(n, normidx); vpack.next(1);
      vpack.WriteVector4(n, normidx); vpack.next(1);
    } 
  }

  IndicesDataPtr idata = CreateBoxFrameIndicesData();
  
  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  frame_ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}

FrustrumObject::~FrustrumObject() {
}

MeshPartPtr FrustrumObject::CreateFrameObject(Effect* effect) {
  EntityPtr entity(new Entity(vb_, frame_ib_));
  entity->set_topology(kLineList);
  MeshPartPtr part(new MeshPart(effect));
  part->AddEntity(entity);
  return part;
}

void FrustrumObject::Render(Renderer* renderer) {
  renderer->UseVertexBuffer(vb_.get());
  renderer->SetPrimitiveTopology(kTriangleList);
  renderer->Draw(0, vb_->vertex_num());
}
}  // namespace azer
