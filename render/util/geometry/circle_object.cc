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
  const int32 kNormalIndex = GetSemanticIndex("normal", 0, desc_.get());
  const int32 kVertexNum = kSlice + 1;
  float degree = 360.0f / (float)kSlice;
  VertexDataPtr vdata(new VertexData(desc_, kVertexNum));
  VertexPack vpack(vdata.get());
  vpack.first();
  for (int i = 0; i < kVertexNum; ++i) {
    float x = cos(Degree(i * degree));
    float y = sin(Degree(i * degree));
    vpack.WriteVector4(Vector4(x, y, 0.0f, 1.0f), 0);
    if (kNormalIndex > 0) {
      vpack.WriteVector4(Vector4(0.0f, 0.0f, 1.0f, 0.0f), kNormalIndex);
    }
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
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata, 0);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}

void CircleObject::Render(Renderer* renderer) {
  renderer->DrawIndex(vb_.get(), ib_.get(), kLineList);
}

void CircleObject::RenderWireframe(Renderer* renderer) {
  renderer->DrawIndex(vb_.get(), ib_.get(), kLineList);
}
}  // namespace azer
