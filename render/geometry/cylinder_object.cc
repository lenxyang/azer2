#include "azer/render/geometry/cylinder_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/index_pack.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/geometry/util.h"
#include "azer/render/geometry/normal_util.h"

namespace azer {
namespace {
int32 CalcCylinderIndexNum(int32 stack_num, int32 slice_num) {
  return (stack_num - 1) * slice_num * 3 * 2 + slice_num * 2 * 3;
}

int32 CalcCylinderVertexNum(int32 stack_num, int32 slice_num) {
  return stack_num * slice_num + 2;
}
}
CylinderObject::CylinderObject(VertexDescPtr desc, 
                               float top_radius, float bottom_radius, float height,
                               int32 stack, int32 slice, bool withhat)
    : GeometryObject(desc),
      stack_(stack),
      slice_(slice),
      top_radius_(top_radius),
      bottom_radius_(bottom_radius),
      height_(height),
      withhat_(withhat) {
  InitHardwareBuffers();
  float max_x = std::max(top_radius_, bottom_radius_);
  float min_x = std::max(top_radius_, bottom_radius_);
  vmin_ = Vector3(min_x,  0.0f, min_x);
  vmax_ = Vector3(max_x,  1.0f, max_x);
}

CylinderObject::~CylinderObject() {
}

void CylinderObject::InitHardwareBuffers() {
  SlotVertexDataPtr vdata = InitCylinderVertexData(
      top_radius_, bottom_radius_, height_, stack_, slice_, withhat_, desc_.get());
  IndicesDataPtr idata = InitCylinderIndicesData(stack_, slice_, withhat_);
  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}

SlotVertexDataPtr InitCylinderVertexData(
    float top_radius, float bottom_radius, float height, int32 stack, int32 slice,
    bool withhat, VertexDescPtr desc) {
  int32 kVertexCount =  stack * slice;
  int32 kIndicesCount = (stack - 1) * slice* 3 * 2;
  if (withhat) {
    kVertexCount += (slice  + 1) * 2;
    kIndicesCount += slice * 2 * 3;
  }
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kVertexCount));
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  VertexPack vpack(vdata.get());
  IndexPack ipack(idata.get());

  vpack.first();
  
  GenerateBarrel(top_radius, bottom_radius, height, stack, slice,
                 &vpack, &ipack);
  if (withhat) {
    GenerateConeHat(true, height, height, top_radius, slice, &vpack, &ipack);
    GenerateConeHat(false, 0.0f, 0.0f, bottom_radius, slice, &vpack, &ipack);
  }

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

IndicesDataPtr InitCylinderIndicesData(int32 stack, int32 slice, bool withhat) {
  VertexDescPtr desc = CreateVertexDesc();
  int32 kVertexCount = stack * slice;
  int32 kIndicesCount = (stack - 1) * slice* 3 * 2;
  if (withhat) {
    kVertexCount += (slice  + 1) * 2;
    kIndicesCount += slice * 2 * 3;
  }
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kVertexCount));
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  VertexPack vpack(vdata.get());
  IndexPack ipack(idata.get());

  vpack.first();
  GenerateBarrel(1.0f, 1.0f, 1.0f, stack, slice, &vpack, &ipack);
  if (withhat) {
    GenerateConeHat(true, 1.0f, 1.0f, 1.0f, slice, &vpack, &ipack);
    GenerateConeHat(false, 0.0f, 0.0f, 1.0f, slice, &vpack, &ipack);
  }

  return idata;
}
}  // namespace azer
