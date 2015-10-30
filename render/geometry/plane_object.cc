#include "azer/render/geometry/plane_object.h"

#include "azer/render/geometry/util.h"
#include "azer/render/render_system.h"

namespace azer {
PlaneObject::PlaneObject(VertexDescPtr desc, int32 row, int column)
    : GeometryObject(desc),
      kRowLine(row + 1),
      kColumnLine(column + 1) {
  InitHardwareBuffers();
}

PlaneObject::~PlaneObject() {
}

void PlaneObject::InitHardwareBuffers() {
  RenderSystem* rs = RenderSystem::Current();
  VertexPos npos, tpos;
  const bool kHasNormalIndex = GetSemanticIndex("normal", 0, desc_.get(), &npos);
  const bool kHasTexcoordIndex = GetSemanticIndex("texcoord", 0, desc_.get(), &tpos);

  float row_width = 2.0f / (kRowLine - 1);
  float column_width = 2.0f / (kColumnLine - 1);
  SlotVertexDataPtr vdata(new SlotVertexData(desc_, kRowLine * kColumnLine));
  VertexPack vpack(vdata.get());
  vpack.first();
  for (int i = 0; i < kRowLine; ++i) {
    for (int j = 0; j < kColumnLine; ++j) {
      float x = -1.0 + j * column_width;
      float y = 1.0 - i * row_width;
      vpack.WriteVector4(Vector4(x, y, 0.0f, 1.0f), VertexPos(0, 0));
      vpack.WriteVector4(Vector4(0.0f, 0.0f, 1.0f, 0.0f), npos);

      float tu = (x + 1.0) * 0.5;
      float tv = (1.0 - y) * 0.5;
      vpack.WriteVector2(Vector2(tu, tv), tpos);
      vpack.next(1);
    }
  }
  CHECK(vpack.end());
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);

  {
    const int32 kIndexNum = (kRowLine - 1) * (kColumnLine - 1) * 2 * 3;
    IndicesDataPtr idata(new IndicesData(kIndexNum));
    IndexPack ipack(idata.get());
    for (int i = 0; i < kRowLine - 1; ++i) {
      for (int j = 0; j < kColumnLine - 1; ++j) {
        int cur_line = i * kColumnLine;
        int next_line = (i + 1) * kColumnLine;
        CHECK(ipack.WriteAndAdvance(cur_line  + j));
        CHECK(ipack.WriteAndAdvance(next_line + j));
        CHECK(ipack.WriteAndAdvance(next_line + j + 1));
        CHECK(ipack.WriteAndAdvance(cur_line  + j));
        CHECK(ipack.WriteAndAdvance(next_line + j + 1));
        CHECK(ipack.WriteAndAdvance(cur_line  + j + 1));
      }
    }
    ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
  }

  {
    int32 count = kRowLine * 2 + kColumnLine * 2;
	IndicesDataPtr idata(new IndicesData(count));
    IndexPack ipack(idata.get());
    for (uint32 i = 0; i < kRowLine; ++i) {
      int32 index1 = i * kColumnLine;
      int32 index2 = (i  + 1) * kColumnLine - 1;
      CHECK(ipack.WriteAndAdvance(index1));
      CHECK(ipack.WriteAndAdvance(index2));
    }

    for (uint32 i = 0; i < kColumnLine; ++i) {
      int32 index1 = i;
      int32 index2 = (kRowLine  - 1) * kColumnLine + i;
      CHECK(ipack.WriteAndAdvance(index1));
      CHECK(ipack.WriteAndAdvance(index2));
    }
    frame_ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
  }
}
}  // namespace azer
