#include "azer/render/geometry/cone_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/index_pack.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/geometry/util.h"

namespace azer {
ConeObject::ConeObject(VertexDescPtr desc)
    : GeometryObject(desc),
      slice_(24) {
  InitHardwareBuffers();
}

ConeObject::ConeObject(VertexDescPtr desc, int32 slice)
    : GeometryObject(desc),
      slice_(slice) {
  InitHardwareBuffers();
}

ConeObject::~ConeObject() {
}

void ConeObject::InitHardwareBuffers() {
  int32 kVertexCount = (slice_  + 1) * 2;
  int32 kIndicesCount = slice_ * 3 * 2;
  SlotVertexDataPtr vdata(new SlotVertexData(desc_, kVertexCount));
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  VertexPack vpack(vdata.get());
  IndexPack ipack(idata.get());

  vpack.first();
  GenerateConeHat(true, 1.0f, 0.0f, slice_, &vpack, &ipack);
  GenerateConeHat(false, 0.0f, 0.0f, slice_, &vpack, &ipack);

  VertexPos pos;
  if (GetSemanticIndex("normal", 0, desc_.get(), &pos)) {
    CalcNormal(vdata.get(), idata.get());
  }

  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}
}  // namespace azer
