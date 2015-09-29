#pragma once

#include "azer/render/render.h"
#include "azer/render/util/geometry/geometry_object.h"

namespace azer {

class CylinderObject : public GeometryObject {
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

VertexDataPtr InitCylinderVertexData(float top_radius, float bottom_radius,
                                     int32 stack, int32 slice, VertexDescPtr desc); 
IndicesDataPtr InitCylinderIndicesData(int32 stack, int32 slice); 

}  // namespace azer
