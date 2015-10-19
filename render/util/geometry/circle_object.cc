#include "azer/render/util/geometry/circle_object.h"

#include "azer/render/render.h"
#include "azer/render/util/geometry/util.h"

namespace azer {
CircleObject::CircleObject(VertexDescPtr desc) 
    : GeometryObject(desc),
      kSlice(32) {
  InitHardwareBuffers();
}

CircleObject::CircleObject(VertexDescPtr desc, int32 slice) 
    : GeometryObject(desc),
      kSlice(slice) {
  InitHardwareBuffers();
}

CircleObject::~CircleObject() {
}

void CircleObject::InitHardwareBuffers() {
  VertexPos npos;
  GetSemanticIndex("normal", 0, desc_.get(), &npos);
  const int32 kVertexNum = kSlice + 1;
  float degree = 360.0f / (float)kSlice;
  SlotVertexDataPtr vdata(new SlotVertexData(desc_, kVertexNum));
  VertexPack vpack(vdata.get());
  vpack.first();
  for (int i = 0; i < kVertexNum; ++i) {
    float x = cos(Degree(i * degree));
    float y = sin(Degree(i * degree));
    vpack.WriteVector4(Vector4(x, y, 0.0f, 1.0f), VertexPos(0, 0));
    vpack.WriteVector4(Vector4(0.0f, 0.0f, 1.0f, 0.0f), npos);
    vpack.next(1);
  }
  CHECK(vpack.end());

  const int kIndexNum = kSlice * 2;
  IndicesDataPtr idata(new IndicesData(kIndexNum));  
  IndexPack ipack(idata.get());
  for (int i = 0; i < kSlice; ++i) {
    CHECK(ipack.WriteAndAdvance(i));
    CHECK(ipack.WriteAndAdvance((i + 1) % kVertexNum));
  }

  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}

void CircleObject::Render(Renderer* renderer) {
  vb_->Use(renderer);
  ib_->Use(renderer);
  renderer->SetPrimitiveTopology(kTriangleList);
  renderer->DrawIndex(ib_->indices_num(), 0, 0);
}

void CircleObject::RenderWireframe(Renderer* renderer) {
  vb_->Use(renderer);
  ib_->Use(renderer);
  renderer->SetPrimitiveTopology(kLineList);
  renderer->DrawIndex(ib_->indices_num(), 0, 0);
}
}  // namespace azer
