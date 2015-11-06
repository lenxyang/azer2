#include "azer/render/geometry/cylinder_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/index_pack.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/geometry/util.h"

namespace azer {
namespace {
int32 CalcCylinderIndexNum(int32 stack_num, int32 slice_num) {
  return (stack_num - 1) * slice_num * 3 * 2 + slice_num * 2 * 3;
}

int32 CalcCylinderVertexNum(int32 stack_num, int32 slice_num) {
  return stack_num * slice_num + 2;
}
}
CylinderObject::CylinderObject(VertexDescPtr desc)
    : GeometryObject(desc),
      stack_(12),
      slice_(24),
      top_radius_(1.0f),
      bottom_radius_(1.0f) {
  InitHardwareBuffers();
}

CylinderObject::CylinderObject(VertexDescPtr desc, int32 stack, int32 slice)
    : GeometryObject(desc),
      stack_(stack),
      slice_(slice),
      top_radius_(1.0f),
      bottom_radius_(1.0f) {
  InitHardwareBuffers();
}

CylinderObject::CylinderObject(VertexDescPtr desc, 
                               float top_radius, float bottom_radius,
                               int32 stack, int32 slice)
    : GeometryObject(desc),
      stack_(stack),
      slice_(slice),
      top_radius_(top_radius),
      bottom_radius_(bottom_radius) {
  InitHardwareBuffers();
}

CylinderObject::CylinderObject(VertexDescPtr desc, 
                               float top_radius, float bottom_radius)
    : GeometryObject(desc),
      stack_(12),
      slice_(24),
      top_radius_(top_radius),
      bottom_radius_(bottom_radius) {
  InitHardwareBuffers();
}

CylinderObject::~CylinderObject() {
}

void CylinderObject::InitHardwareBuffers() {
  SlotVertexDataPtr vdata = InitCylinderVertexData(top_radius_, 
                                                   bottom_radius_,
                                                   stack_, slice_,
                                                   desc_.get());
  IndicesDataPtr idata = InitCylinderIndicesData(stack_, slice_);
  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}

SlotVertexDataPtr InitCylinderVertexData(float top_radius, 
                                         float bottom_radius,
                                         int32 stack, int32 slice,
                                         VertexDescPtr desc) {
  int32 kVertexCount = (slice  + 1) * 2 + stack * slice;
  int32 kIndicesCount = (stack - 1) * slice* 3 * 2 + slice * 2 * 3;
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kVertexCount));
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  VertexPack vpack(vdata.get());
  IndexPack ipack(idata.get());

  vpack.first();
  GenerateConeHat(true, 1.0f, 1.0f, slice, &vpack, &ipack);
  GenerateBarrel(top_radius, bottom_radius, 1.0f, stack, slice,
                 &vpack, &ipack);
  GenerateConeHat(false, 0.0f, 0.0f, slice, &vpack, &ipack);

  VertexPos npos;
  if (GetSemanticIndex("normal", 0, desc, &npos)) {
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

IndicesDataPtr InitCylinderIndicesData(int32 stack, int32 slice) {
  VertexDescPtr desc = CreateVertexDesc();
  int32 kVertexCount = (slice  + 1) * 2 + stack * slice;
  int32 kIndicesCount = (stack - 1) * slice* 3 * 2 + slice * 2 * 3;
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kVertexCount));
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  VertexPack vpack(vdata.get());
  IndexPack ipack(idata.get());

  vpack.first();
  GenerateConeHat(true, 1.0f, 1.0f, slice, &vpack, &ipack);
  GenerateBarrel(1.0f, 1.0f, 1.0f, stack, slice, &vpack, &ipack);
  GenerateConeHat(false, 0.0f, 0.0f, slice, &vpack, &ipack);

  return idata;
}
}  // namespace azer
