#include "azer/render/geometry/sphere_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"
#include "azer/render/geometry/util.h"
#include "azer/render/geometry/normal_util.h"

namespace azer {

namespace {
inline int32 CalcSphereIndexNum(int32 stack_num, int32 slice_num) {
  return (stack_num - 2 - 1) * slice_num * 3 * 2 + slice_num * 2 * 3;
}

inline int32 CalcSphereVertexNum(int32 stack_num, int32 slice_num) {
  return (stack_num - 2) * slice_num + 2;
}
}  // namespace


SlotVertexDataPtr InitSphereVertexData(int32 stack, int32 slice, 
                                       float rad, VertexDescPtr desc) {
  const int32 kVertexNum = CalcSphereVertexNum(stack, slice);
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kVertexNum));
  VertexPack vpack(vdata.get());

  int num = 0;
  CHECK(vpack.first());
  vpack.WriteVector4(Vector4(0.0f, rad, 0.0f, 1.0f), VertexPos(0, 0));
  num++;

  for (int i = 1; i < stack - 1; ++i) {
    float y = rad * sin(Degree(90.0f - i * 180.0f / (float)stack));
    float slice_radius =  cos(Degree(90.0f - i * 180.0f / (float)stack));
    for (int j = 0; j < slice; ++j) {
      float degree = 360.0f - j * 360.0f / slice;
      float x = rad * slice_radius * cos(Degree(degree));
      float z = rad * slice_radius * sin(Degree(degree));

      CHECK(vpack.next(1));
      vpack.WriteVector4(Vector4(x, y, z, 1.0f), VertexPos(0, 0));
      num++;
    }
  }

  CHECK(vpack.next(1));
  vpack.WriteVector4(Vector4(0.0f, -rad, 0.0f, 1.0f), VertexPos(0, 0));
  num++;
  DCHECK_EQ(num, kVertexNum);
  return vdata;
}

IndicesDataPtr InitSphereIndicesData(int32 stack, int32 slice) {
  const int kIndexNum = CalcSphereIndexNum(stack, slice);
  const int32 kVertexNum = CalcSphereVertexNum(stack, slice);
  int bottom_index = kVertexNum - 1;
  IndicesDataPtr idata(new IndicesData(kIndexNum));  
  IndexPack ipack(idata.get());
  CHECK(GenerateTopTriangleStrip(0, 1, slice, true, &ipack));
  for (int i = 1; i < stack - 2; ++i) {
    CHECK(GenerateStripIndex(1 + slice * (i - 1), 1 + slice * i, slice, true,
                             &ipack));
  }
  CHECK(GenerateBottomTriangleStrip(bottom_index, bottom_index - slice,
                                    slice, true, &ipack));
  CHECK(ipack.count() == kIndexNum);
  return idata;
}

IndicesDataPtr InitSphereWireFrameIndicesData(int32 stack, int32 slice) {
  const int kIndexNum = (stack - 1) * slice * 2 + (stack - 2) * (slice + 1) * 2;
  const int32 kVertexNum = CalcSphereVertexNum(stack, slice);
  IndicesDataPtr idata(new IndicesData(kIndexNum));  
  IndexPack ipack(idata.get());
  for (int i = 0; i < slice; ++i) {
    CHECK(ipack.WriteAndAdvance(0));
    CHECK(ipack.WriteAndAdvance(1 + i));
  }

  for (int i = 1; i < stack - 2; ++i) {
    for (int j = 0; j < slice; ++j) {
      CHECK(ipack.WriteAndAdvance(1 + (i - 1) * slice + j));
      CHECK(ipack.WriteAndAdvance(1 + i * slice + j));
    }
  }

  for (int i = 0; i < slice; ++i) {
    CHECK(ipack.WriteAndAdvance(kVertexNum - 1));
    CHECK(ipack.WriteAndAdvance(kVertexNum - 1 - (i + 1)));
  }

  for (int i = 1; i < stack - 1; ++i) {
    for (int j = 0; j < slice; ++j) {
      CHECK(ipack.WriteAndAdvance(1 + (i - 1) * slice + j));
      CHECK(ipack.WriteAndAdvance(1 + (i - 1) * slice + (j + 1) % slice));
    }
  }
  CHECK(ipack.count()  == kIndexNum);
  return idata;
}


SphereObject::SphereObject(VertexDescPtr desc, float radius, int32 stack, 
                           int32 slice)
    : GeometryObject(desc),
      radius_(radius),
      stack_(stack), 
      slice_(slice) {
  InitHardwareBuffers();
}

SphereObject::~SphereObject() {
}

void SphereObject::InitHardwareBuffers() {
  SlotVertexDataPtr vdata(InitSphereVertexData(stack_, slice_, radius_, desc_));
  IndicesDataPtr idata = InitSphereIndicesData(stack_, slice_);

  VertexPos npos;
  if (GetSemanticIndex("normal", 0, desc_.get(), &npos)) {
    CalcNormal(vdata.get(), idata.get());
  }

  IndicesDataPtr edge_idata = InitSphereWireFrameIndicesData(stack_, slice_);
  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
  frame_ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), edge_idata);
  vmin_ = Vector3(-0.5f, -0.5f, -0.5f);
  vmax_ = Vector3( 0.5f,  0.5f,  0.5f);
}

}  // namespace azer
