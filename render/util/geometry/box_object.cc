#include "azer/render/util/geometry/box_object.h"

#include "azer/math/math.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"
#include "azer/render/util/vertex_pack.h"
#include "azer/render/util/index_pack.h"
#include "azer/render/util/effects/pvw_effect.h"
#include "azer/render/util/geometry/util.h"

namespace azer {

BoxObject::BoxObject(VertexDescPtr desc)
    : GeometryObject(desc) {
  InitHardwareBuffers();
}

BoxObject::~BoxObject() {
}

void BoxObject::InitHardwareBuffers() {
  const Vector4 position[] = {
    Vector4(-0.5f,  0.5f,  0.5f, 1.0f),
    Vector4( 0.5f,  0.5f,  0.5f, 1.0f),
    Vector4( 0.5f, -0.5f,  0.5f, 1.0f),
    Vector4(-0.5f, -0.5f,  0.5f, 1.0f),
    Vector4(-0.5f,  0.5f, -0.5f, 1.0f),
    Vector4( 0.5f,  0.5f, -0.5f, 1.0f),
    Vector4( 0.5f, -0.5f, -0.5f, 1.0f),
    Vector4(-0.5f, -0.5f, -0.5f, 1.0f),
  };

  const Vector2 texcoord0[] = {
    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 1.0f),
    Vector2(1.0f, 0.0f),
    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 0.0f),
    Vector2(1.0f, 1.0f),

    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 1.0f),
    Vector2(1.0f, 0.0f),
    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 0.0f),
    Vector2(1.0f, 1.0f),

    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 1.0f),
    Vector2(1.0f, 0.0f),
    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 0.0f),
    Vector2(1.0f, 1.0f),

    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 1.0f),
    Vector2(1.0f, 0.0f),
    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 0.0f),
    Vector2(1.0f, 1.0f),

    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 1.0f),
    Vector2(1.0f, 0.0f),
    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 0.0f),
    Vector2(1.0f, 1.0f),

    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 1.0f),
    Vector2(0.0f, 1.0f),
    Vector2(0.0f, 0.0f),
    Vector2(1.0f, 0.0f),
    Vector2(1.0f, 1.0f),
  };
  int indices[] = {0, 2, 1, 0, 3, 2,  // front
                   1, 6, 5, 1, 2, 6,  // right
                   5, 7, 4, 5, 6, 7,  // back
                   4, 3, 0, 4, 7, 3,  // left
                   4, 1, 5, 4, 0, 1,  // top
                   3, 6, 2, 3, 7, 6}; // bottom
  Vector4 normal[] = {
    Vector4(0.0f, 0.0f, 1.0f, 0.0f),
    Vector4(1.0f, 0.0f, 0.0f, 0.0f),
    Vector4(0.0f, 0.0f, -1.0f, 0.0f),
    Vector4(-1.0f, 0.0f, 0.0f, 0.0f),
    Vector4(0.0f,  1.0f, 0.0f, 0.0f),
    Vector4(0.0f,  -1.0f, 0.0f, 0.0f),
  };
    

  int32 kNormal0Idx = GetSemanticIndex("normal", 0, desc_.get());
  int32 kTexcoord0Idx = GetSemanticIndex("texcoord", 0, desc_.get());
  VertexDataPtr vdata(new VertexData(desc_, arraysize(indices)));
  VertexPack vpack(vdata.get());
  vpack.first();
  for (int i = 0; i < static_cast<int>(arraysize(indices)); ++i) {
    int index = indices[i];
    DCHECK(!vpack.end());
    vpack.WriteVector4(position[index], 0);
	if (kTexcoord0Idx > 0)
      vpack.WriteVector2(texcoord0[index], kTexcoord0Idx);
    vpack.next(1);
  }
  DCHECK(vpack.end());

  if (kNormal0Idx > 0) {
    vpack.first(); 
    for (int i = 0; i < static_cast<int>(arraysize(indices)); i += 6) {
      int index = i / arraysize(normal);
      for (int j = 0; j < 6; ++j) { 
        vpack.WriteVector4(normal[index], kNormal0Idx);
        vpack.next(1);
      }
    }
  }
  
  int32 edge_indices[] = {0, 2, 2, 1, 1, 4, 4, 0,
                          0, 14, 2, 8, 1, 7, 4, 13,
                          14, 8, 8, 7, 7, 13, 13, 14};
  IndicesDataPtr idata(new IndicesData(arraysize(edge_indices)));
  IndexPack ipack(idata.get());
  for (uint32 i = 0; i < arraysize(edge_indices); ++i) {
    CHECK(ipack.WriteAndAdvance(edge_indices[i]));
  }

  RenderSystem* rs = RenderSystem::Current();
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  edge_ib_ = rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}

void BoxObject::Render(Renderer* renderer) {
  renderer->Draw(vb_.get(), kTriangleList);
}

}  // namespace azer

