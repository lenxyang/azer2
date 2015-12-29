#include "azer/render/geometry/frustum_object.h"

#include "base/logging.h"
#include "azer/render/render.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"
#include "azer/render/geometry/box_object.h"

namespace azer {
FrustumObject::FrustumObject(const Camera* camera, VertexDescPtr desc)
    : GeometryObject(desc) {
  const Matrix4& proj = camera->frustum().projection();
  const Matrix4& view = camera->GetViewMatrix();
  Matrix4 mat = std::move(view.InverseCopy() * proj.InverseCopy());
  SlotVertexDataPtr vdata = CreateBoxVertexData(desc.get());
  VertexPack vpack(vdata.get());
  VertexPos posidx;
  CHECK(GetSemanticIndex("position", 0, desc_.get(), &posidx));
  Vector4 v;
  vpack.first();
  while (!vpack.end()) {
    vpack.ReadVector4(&v, posidx);
    v.x *= 2.0f;
    v.y *= 2.0f;
    v.z += 0.5f;
    v.z *= 0.92f;

    Vector4 test = v;
    v = mat * v;
    v /= v.w;
    vpack.WriteVector4(v, posidx);
    vpack.next(1);
  }

  {
    vpack.first();
    Vector4 v1, v2, v3;
    while (!vpack.end()) {
      vpack.ReadVector4(&v1, posidx); vpack.next(1);
      vpack.ReadVector4(&v2, posidx); vpack.next(1);
      vpack.ReadVector4(&v3, posidx); vpack.next(1);

      vpack.next(-3);
      vpack.WriteVector4(v3, posidx); vpack.next(1);
      vpack.WriteVector4(v2, posidx); vpack.next(1);
      vpack.WriteVector4(v1, posidx); vpack.next(1);
    }
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

  // because of triangle has been revese, so need some operation on indices
  int32 edge_indices[] = {0, 2, 2, 1, 1, 4, 4, 0,
                          0, 14, 2, 8, 1, 7, 4, 13,
                          14, 8, 8, 7, 7, 13, 13, 14};
  IndicesDataPtr idata(new IndicesData(arraysize(edge_indices)));
  IndexPack ipack(idata.get());
  for (uint32 i = 0; i < arraysize(edge_indices); ++i) {
    int32 index = edge_indices[i];
    if (index % 3 == 0) { index += 2;}
    else if (index % 3 == 2) { index -= 2;}
    CHECK(ipack.WriteAndAdvance(index));
  }

  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  frame_ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}

FrustumObject::~FrustumObject() {
}

MeshPartPtr FrustumObject::CreateFrameObject(Effect* effect) {
  EntityPtr entity(new Entity(effect->vertex_desc(), vb_, frame_ib_));
  entity->set_topology(kLineList);
  MeshPartPtr part(new MeshPart(effect));
  part->AddEntity(entity);
  return part;
}

void FrustumObject::Render(Renderer* renderer) {
  renderer->UseVertexBuffer(vb_.get());
  renderer->SetPrimitiveTopology(kTriangleList);
  renderer->Draw(0, vb_->vertex_count());
}
}  // namespace azer
