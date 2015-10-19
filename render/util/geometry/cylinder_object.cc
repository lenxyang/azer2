#include "azer/render/util/geometry/cylinder_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/util/index_pack.h"
#include "azer/render/util/vertex_pack.h"
#include "azer/render/util/effects/pvw_effect.h"
#include "azer/render/util/geometry/util.h"

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
  int32 kVertexCount = (slice_  + 1) * 2 + stack_ * slice_;
  int32 kIndicesCount = (stack_ - 1) * slice_* 3 * 2 + slice_ * 2 * 3;
  VertexDataPtr vdata(new VertexData(desc_, kVertexCount));
  IndicesDataPtr idata(new IndicesData(kIndicesCount));  
  VertexPack vpack(vdata.get());
  IndexPack ipack(idata.get());

  vpack.first();
  GenerateConeHat(true, 1.0f, 1.0f, slice_, &vpack, &ipack);
  GenerateBarrel(top_radius_, bottom_radius_, 1.0f, stack_, slice_,
                 &vpack, &ipack);
  GenerateConeHat(false, 0.0f, 0.0f, slice_, &vpack, &ipack);
  if (GetSemanticIndex("normal", 0, desc_.get()) > 0) {
    CalcNormal(vdata.get(), idata.get());
  }

  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata, 0);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}
}  // namespace azer
