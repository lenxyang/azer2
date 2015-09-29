#include "azer/render/util/geometry/cone_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/util/index_pack.h"
#include "azer/render/util/vertex_pack.h"
#include "azer/render/util/effects/pvw_effect.h"
#include "azer/render/util/geometry/indices_util.h"

namespace azer {
VertexDataPtr InitConeVertexData(int32 slice, VertexDescPtr desc) {
  int32 kVertexNum = slice + 2;
  VertexDataPtr vbd(new VertexData(desc, kVertexNum));
  VertexPack vdata(vbd.get());
  int num = 0;
  CHECK(vdata.first());
  vdata.WriteVector4(Vector4(0.0f, 1.0, 0.0f, 1.0f), 0);
  num++;
  
  float slice_degree = 360.0f / slice;
  for (int32 i = 0; i < slice; ++i) {
    float degree = 360.0f - slice_degree *  i;
    float x = azer::cos(Degree(degree));
    float z = azer::sin(Degree(degree));

    CHECK(vdata.next(1));
    vdata.WriteVector4(Vector4(x, 0.0f, z, 1.0f), 0);
    num++;
  }

  CHECK(vdata.next(1));
  vdata.WriteVector4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0);
  num++;
  CHECK_EQ(num, kVertexNum);
  return vbd;
}

IndicesDataPtr InitConeIndicesData(int32 slice) {
  int32 kIndexNum = slice * 2 * 3;
  IndicesDataPtr idata(new IndicesData(kIndexNum, IndicesData::kUint32));  
  IndexPack ipack(idata.get());
  CHECK(GenerateTopTriangleStrip(0, 1, slice, true, &ipack));
  DCHECK(GenerateBottomTriangleStrip(slice + 1, 1, slice, true, &ipack));
  DCHECK_EQ(ipack.count(), kIndexNum);
  return idata;
}

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
  VertexDataPtr vdata(InitConeVertexData(slice_, desc_));
  IndicesDataPtr idata = InitConeIndicesData(slice_);

  if (GetSemanticIndex("normal", 0, desc_.get()) > 0) {
    CalcNormal(vdata.get(), idata.get());
  }

  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}
}  // namespace azer
