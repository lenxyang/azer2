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
  RenderSystem* rs = RenderSystem::Current();
  SlotVertexDataPtr vdata = InitConeVertexData(slice_, desc_);
  IndicesDataPtr idata = InitConeIndicesData(slice_);
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);

  vmin_ = Vector3(-1.0f,  0.0f, -1.0f);
  vmax_ = Vector3( 1.0f,  1.0f,  1.0f);
}

SlotVertexDataPtr InitConeVertexData(int32 slice, VertexDescPtr desc) {
  int32 kVertexCount = (slice  + 1) * 2;
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kVertexCount));
  VertexPack vpack(vdata.get());
  int32 kIndicesCount = slice * 3 * 2;
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  IndexPack ipack(idata.get());

  vpack.first();
  GenerateConeHat(true, 1.0f, 0.0f, slice, &vpack, &ipack);
  GenerateConeHat(false, 0.0f, 0.0f, slice, &vpack, &ipack);

  VertexPos pos;
  if (GetSemanticIndex("normal", 0, desc.get(), &pos)) {
    CalcNormal(vdata.get(), idata.get());
  }

  return vdata;
}

namespace {
// class PositionVertex
const VertexDesc::Desc kVertexDesc[] = {
  {"POSITION", 0, kVec4},
};

const int kVertexDescNum = arraysize(kVertexDesc);
VertexDescPtr CreateVertexDesc() {
  return VertexDescPtr(new VertexDesc(kVertexDesc, kVertexDescNum));
}
}  // namespace

IndicesDataPtr InitConeIndicesData(int32 slice) {
  VertexDescPtr desc = CreateVertexDesc();
  int32 kVertexCount = (slice  + 1) * 2;
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kVertexCount));
  VertexPack vpack(vdata.get());
  int32 kIndicesCount = slice * 3 * 2;
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  IndexPack ipack(idata.get());

  vpack.first();
  GenerateConeHat(true, 1.0f, 0.0f, slice, &vpack, &ipack);
  GenerateConeHat(false, 0.0f, 0.0f, slice, &vpack, &ipack);

  VertexPos pos;
  if (GetSemanticIndex("normal", 0, desc.get(), &pos)) {
    CalcNormal(vdata.get(), idata.get());
  }

  return idata;
}
}  // namespace azer
