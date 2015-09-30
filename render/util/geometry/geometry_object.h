
#pragma once

#include "base/memory/ref_counted.h"
#include "azer/math/math.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/renderer.h"
#include "azer/render/transform_holder.h"
#include "azer/render/util/effects/pvw_effect.h"

namespace azer {

class Camera;
class PVWEffect;

class GeometryObject : public ::base::RefCounted<GeometryObject> {
 public:
  GeometryObject(VertexDescPtr desc);
  ~GeometryObject();

  virtual void Render(Renderer* renderer);
  virtual void RenderWireframe(Renderer* renderer);
 protected:
  VertexDescPtr desc_;
  VertexBufferPtr vb_;
  IndicesBufferPtr ib_;
  IndicesBufferPtr edge_ib_;
  DISALLOW_COPY_AND_ASSIGN(GeometryObject);
};

typedef scoped_refptr<GeometryObject> GeometryObjectPtr;
}  // namespace azer
