#include "azer/render/util/geometry/plane_object.h"

namespace azer {
PlaneObject::PlaneObject(VertexDescPtr desc, int32 row, int column)
    : GeometryObject(desc),
      kRowLine(row + 1),
      kColumnLine(column + 1) {
}

PlaneObject::~PlaneObject() {
}

void PlaneObject::InitHardwareBuffers() {
  const int32 kNormalIndex = GetSemanticIndex("normal", 0, desc_.get());
  const int32 kTexcoordIndex = GetSemanticIndex("texcoord", 0, desc_.get());

  float row_width = 2.0f / (kRowLine - 1);
  float column_width = 2.0f / (kRowLine - 1);
  VertexDataPtr vdata(new VertexData(desc_, arraysize(kRowLine * kColumnLine)));
  VertexPack vpack(vdata.get());
  vpack.first();
  for (int i = 0; i < kRowLine; ++i) {
    for (int j = 0; j < kColumnLine; ++j) {
      float x = -1.0 + j * column_width;
      float y = 1.0 - i * row_width;
      vpack.WriteVector4(Vector4(x, y, 0.0f, 1.0f), 0);
      if (kNormalIndex > 0)
        vpack.WriteVector4(Vector4(0.0f, 0.0f, 1.0f, 0.0f), kNormalIndex);

      if (kTexcoordIndex > 0) 
        vpack.WriteVector4(Vector4(0.0f, 0.0f, 1.0f, 0.0f), kTexcoordIndex);
      }
      vpack.next();
    }
  }
  CHECK(vpack.end());

  const int32 kIndexNum = (kRowLine - 1) * (kColumnLine - 1) * 2 * 3;
  IndicesDataPtr idata(new IndicesData(kIndexNum));  
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
}
}  // namespace azer
