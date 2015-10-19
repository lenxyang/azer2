#include "azer/render/util/geometry/cone_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/util/index_pack.h"
#include "azer/render/util/vertex_pack.h"
#include "azer/render/util/effects/pvw_effect.h"
#include "azer/render/util/geometry/util.h"

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
  VertexDataPtr vdata(new VertexData(desc_, kVertexCount));
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  VertexPack vpack(vdata.get());
  IndexPack ipack(idata.get());

  vpack.first();
  GenerateConeHat(true, 1.0f, 0.0f, slice_, &vpack, &ipack);
  GenerateConeHat(false, 0.0f, 0.0f, slice_, &vpack, &ipack);

  if (GetSemanticIndex("normal", 0, desc_.get()) > 0) {
    CalcNormal(vdata.get(), idata.get());
  }

  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata, 0);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}
}  // namespace azer
