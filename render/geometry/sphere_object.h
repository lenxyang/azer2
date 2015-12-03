#pragma once

#include "azer/render/geometry/geometry_object.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"

namespace azer {

class AZER_EXPORT SphereObject : public GeometryObject {
 public:
  SphereObject(VertexDescPtr desc, float radius = 1.0f, int32 stack = 24, 
               int32 slice = 24);
  ~SphereObject();

 private:
  void InitHardwareBuffers();
  float radius_;
  int32 stack_, slice_;
  DISALLOW_COPY_AND_ASSIGN(SphereObject);
};

AZER_EXPORT SlotVertexDataPtr InitSphereVertexData(
    int32 stack, int32 slice, float radius, VertexDescPtr desc); 
AZER_EXPORT IndicesDataPtr InitSphereIndicesData(int32 stack, int32 slice); 
AZER_EXPORT IndicesDataPtr InitSphereWireFrameIndicesData(int32 stack, int32 slice);

}  // namespace azer
