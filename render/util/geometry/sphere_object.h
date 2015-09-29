#pragma once

#include "azer/render/util/geometry/geometry_object.h"
#include "azer/render/util/vertex_pack.h"
#include "azer/render/util/index_pack.h"

namespace azer {

class SphereObject : public GeometryObject {
 public:
  SphereObject(VertexDescPtr desc);
  SphereObject(VertexDescPtr desc, int32 stack, int32 slice);
  ~SphereObject();

 private:
  void InitHardwareBuffers();
  int32 stack_, slice_;
  DISALLOW_COPY_AND_ASSIGN(SphereObject);
};

VertexDataPtr InitSphereVertexData(int32 stack, int32 slice, VertexDescPtr desc); 
IndicesDataPtr InitSphereIndicesData(int32 stack, int32 slice); 
IndicesDataPtr InitSphereWireFrameIndicesData(int32 stack, int32 slice);

}  // namespace azer
