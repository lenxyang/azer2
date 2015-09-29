#include "azer/render/util/geometry/cylinder_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/util/index_pack.h"
#include "azer/render/util/vertex_pack.h"
#include "azer/render/util/effects/pvw_effect.h"
#include "azer/render/util/geometry/indices_util.h"

namespace azer {
namespace {
int32 CalcCylinderIndexNum(int32 stack_num, int32 slice_num) {
  return (stack_num - 1) * slice_num * 3 * 2 + slice_num * 2 * 3;
}

int32 CalcCylinderVertexNum(int32 stack_num, int32 slice_num) {
  return stack_num * slice_num + 2;
}
}
VertexDataPtr InitCylinderVertexData(float top_radius, float bottom_radius, 
                                     float height,
                                     int32 stack, int32 slice, VertexDescPtr desc) {
  int32 kVertexNum = CalcCylinderVertexNum(stack, slice);
  const int32 kPositionIdx = 0;
  const int32 kTexcoord0Idx = GetSemanticIndex("texcoord", 0, desc.get());
  VertexDataPtr vbd(new VertexData(desc, kVertexNum));
  VertexPack vpack(vbd.get());
  int num = 0;
  CHECK(vpack.first());
  vpack.WriteVector4(Vector4(0.0f, height, 0.0f, 1.0f), kPositionIdx);
  if (kTexcoord0Idx > 0) { 
    vpack.WriteVector2(Vector2(0.0f, 0.0f), kTexcoord0Idx); 
  }
  num++;

  float height_unit = height / ((float)stack - 1.0f);
  float radius_unit = (bottom_radius - top_radius) / (float)stack;
  float slice_radius = top_radius;
  float y = height;
  float tex_u_unit = 1.0f / slice;
  float tex_v_unit = 1.0f / (stack + 2.0f);
  
  for (int i = 0; i < stack; ++i) {
    slice_radius += radius_unit;
    for (int j = 0; j < slice; ++j) {
      float degree = 360.0f - j * 360.0f / slice;
      float x = slice_radius * cos(Degree(degree));
      float z = slice_radius * sin(Degree(degree));

      CHECK(vpack.next(1));
      vpack.WriteVector4(Vector4(x, y, z, 1.0f), kPositionIdx);
      float u = j * tex_u_unit;
      float v = (i + 1) * tex_v_unit;
      if (kTexcoord0Idx > 0) { 
        vpack.WriteVector2(Vector2(0.0f, 0.0f),kTexcoord0Idx); 
      }
      num++;
    }
    y -= height_unit;
  }

  CHECK(vpack.next(1));
  vpack.WriteVector4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), kPositionIdx);
  if (kTexcoord0Idx > 0) { 
    vpack.WriteVector2(Vector2(0.0f, 0.0f),kTexcoord0Idx); 
  }
  num++;

  DCHECK_EQ(num, kVertexNum);
  return vbd;
}

IndicesDataPtr InitCylinderIndicesData(int32 stack, int32 slice) {
  int32 kIndexNum = CalcCylinderIndexNum(stack, slice);
  const int32 kVertexNum = CalcCylinderVertexNum(stack, slice);
  int bottom_index = kVertexNum - 1;
  IndicesDataPtr idata(new IndicesData(kIndexNum, IndicesData::kUint32));  
  int32* indices_buffer = (int32*)idata->pointer();
  IndexPack ipack(idata.get());
  CHECK(GenerateTopTriangleStrip(0, 1, slice, true, &ipack));
  for (int i = 0; i < stack - 1; ++i) {
    CHECK(GenerateStripIndex(1 + slice * i, 1 + slice * (i + 1), slice, true,
                             &ipack));
  }
  CHECK(GenerateBottomTriangleStrip(bottom_index, bottom_index - slice,
                                    slice, true, &ipack));
  CHECK(ipack.count() == kIndexNum);
  return idata;
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
  VertexDataPtr vdata(InitCylinderVertexData(top_radius_, bottom_radius_, 1.0,
                                             stack_, slice_, desc_));
  IndicesDataPtr idata = InitCylinderIndicesData(stack_, slice_);

  if (GetSemanticIndex("normal", 0, desc_.get()) > 0) {
    CalcNormal(vdata.get(), idata.get());
  }

  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}
}  // namespace azer
