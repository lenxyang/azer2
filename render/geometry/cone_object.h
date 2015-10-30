#pragma once

#include "azer/render/render.h"
#include "azer/render/geometry/geometry_object.h"

namespace azer {

class AZER_EXPORT ConeObject : public GeometryObject {
 public:
  ConeObject(VertexDescPtr desc);
  ConeObject(VertexDescPtr desc, int32 slice);
  ~ConeObject();

 private:
  void InitHardwareBuffers();
  int32  slice_;
  DISALLOW_COPY_AND_ASSIGN(ConeObject);
};

SlotVertexDataPtr AZER_EXPORT InitConeVertexData(int32 slice, VertexDescPtr desc); 
IndicesDataPtr AZER_EXPORT InitConeIndicesData(int32 slice); 

}  // namespace azer
