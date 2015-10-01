#pragma once

#include "azer/render/render.h"
#include "azer/render/util/geometry/geometry_object.h"

namespace azer {

class ConeObject : public GeometryObject {
 public:
  ConeObject(VertexDescPtr desc);
  ConeObject(VertexDescPtr desc, int32 slice);
  ~ConeObject();

 private:
  void InitHardwareBuffers();
  int32  slice_;
  DISALLOW_COPY_AND_ASSIGN(ConeObject);
};

VertexDataPtr InitConeVertexData(int32 slice, VertexDescPtr desc); 
IndicesDataPtr InitConeIndicesData(int32 slice); 

}  // namespace azer