#pragma once

#include "azer/render/geometry/geometry_object.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"

namespace azer {

class AZER_EXPORT SphereObject : public GeometryObject {
 public:
  SphereObject(VertexDescPtr desc);
  SphereObject(VertexDescPtr desc, int32 stack, int32 slice);
  ~SphereObject();

 private:
  void InitHardwareBuffers();
  int32 stack_, slice_;
  DISALLOW_COPY_AND_ASSIGN(SphereObject);
};

SlotVertexDataPtr AZER_EXPORT InitSphereVertexData(int32 stack, int32 slice,
                                       VertexDescPtr desc); 
IndicesDataPtr AZER_EXPORT InitSphereIndicesData(int32 stack, int32 slice); 
IndicesDataPtr AZER_EXPORT InitSphereWireFrameIndicesData(int32 stack, int32 slice);

}  // namespace azer
