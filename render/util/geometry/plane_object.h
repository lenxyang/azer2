#pragma once

#include "azer/render/util/geometry/geometry_object.h"

namespace azer {

class PlaneObject : public GeometryObject {
 public:
  PlaneObject(VertexDescPtr desc, int32 row, int column);
  ~PlaneObject();
 private:
  void InitHardwareBuffers();

  const int32 kRowLine;
  const int32 kColumnLine;
  DISALLOW_COPY_AND_ASSIGN(PlaneObject);
};
}  // namespace azer
