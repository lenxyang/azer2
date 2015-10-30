#pragma once

#include "azer/render/render.h"
#include "azer/render/geometry/geometry_object.h"

namespace azer {

class AZER_EXPORT CylinderObject : public GeometryObject {
 public:
  CylinderObject(VertexDescPtr desc);
  CylinderObject(VertexDescPtr desc, int32 stack, int32 slick);
  CylinderObject(VertexDescPtr desc, float top_radius, float bottom_radius);
  CylinderObject(VertexDescPtr desc, float top_radius, float bottom_radius,
                 int32 stack, int32 slick);
  ~CylinderObject();

 private:
  void InitHardwareBuffers();
  int32 stack_;
  int32 slice_;
  float top_radius_, bottom_radius_; 
  DISALLOW_COPY_AND_ASSIGN(CylinderObject);
};

SlotVertexDataPtr AZER_EXPORT InitCylinderVertexData(float top_radius, 
                                                     float bottom_radius,
                                                     int32 stack, int32 slice,
                                                     VertexDescPtr desc); 
IndicesDataPtr AZER_EXPORT InitCylinderIndicesData(int32 stack, int32 slice); 

}  // namespace azer
