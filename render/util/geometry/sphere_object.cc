#include "azer/render/util/geometry/sphere_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"
#include "azer/render/util/vertex_pack.h"
#include "azer/render/util/index_pack.h"
#include "azer/render/util/effects/pvw_effect.h"
#include "azer/render/util/geometry/util.h"

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
                                   VertexDescPtr desc) {
  const int32 kVertexNum = CalcSphereVertexNum(stack, slice);
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kVertexNum));
  VertexPack vpack(vdata.get());

  int num = 0;
  CHECK(vpack.first());
  vpack.WriteVector4(Vector4(0.0f, 1.0f, 0.0f, 1.0f), 0);
  num++;

  for (int i = 1; i < stack - 1; ++i) {
    float y = sin(Degree(90.0f - i * 180.0f / (float)stack));
    float slice_radius =  cos(Degree(90.0f - i * 180.0f / (float)stack));
    for (int j = 0; j < slice; ++j) {
      float degree = 360.0f - j * 360.0f / slice;
      float x = slice_radius * cos(Degree(degree));
      float z = slice_radius * sin(Degree(degree));

      CHECK(vpack.next(1));
      vpack.WriteVector4(Vector4(x, y, z, 1.0f), 0);
      num++;
    }
  }

  CHECK(vpack.next(1));
  vpack.WriteVector4(Vector4(0.0f, -1.0f, 0.0f, 1.0f), 0);
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

SphereObject::SphereObject(VertexDescPtr desc)
    : GeometryObject(desc),
      stack_(24), 
      slice_(24) {
  InitHardwareBuffers();
}

SphereObject::SphereObject(VertexDescPtr desc, int32 stack, int32 slice)
    : GeometryObject(desc),
      stack_(stack), 
      slice_(slice) {
  InitHardwareBuffers();
}

SphereObject::~SphereObject() {
}

void SphereObject::InitHardwareBuffers() {
  SlotVertexDataPtr vdata(InitSphereVertexData(stack_, slice_, desc_));
  IndicesDataPtr idata = InitSphereIndicesData(stack_, slice_);

  if (GetSemanticIndex("normal", 0, desc_.get()) > 0) {
    CalcNormal(vdata.get(), idata.get());
  }

  IndicesDataPtr edge_idata = InitSphereWireFrameIndicesData(stack_, slice_);
  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
  edge_ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), edge_idata);
}

}  // namespace azer
