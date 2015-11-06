
#pragma once

#include "base/memory/ref_counted.h"
#include "azer/math/math.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/mesh.h"
#include "azer/render/renderer.h"
#include "azer/render/transform_holder.h"

namespace azer {

class Camera;

class AZER_EXPORT GeometryObject : public ::base::RefCounted<GeometryObject> {
 public:
  GeometryObject(VertexDescPtr desc);
  ~GeometryObject();

  VertexBuffer* GetVertexBuffer() { return vb_.get();}
  IndicesBuffer* GetIndicesBuffer() { return ib_.get(); }
  IndicesBuffer* GetFrameIndicesBuffer() { return frame_ib_.get();}

  virtual MeshPartPtr CreateObject(Effect* effect);
  virtual MeshPartPtr CreateFrameObject(Effect* effect);

  virtual void Render(Renderer* renderer);
  virtual void RenderWireframe(Renderer* renderer);
 protected:
  VertexDescPtr desc_;
  VertexBufferPtr vb_;
  IndicesBufferPtr ib_;
  IndicesBufferPtr frame_ib_;
  DISALLOW_COPY_AND_ASSIGN(GeometryObject);
};

typedef scoped_refptr<GeometryObject> GeometryObjectPtr;
}  // namespace azer
