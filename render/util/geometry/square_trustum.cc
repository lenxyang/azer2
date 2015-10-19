#include "azer/render/util/geometry/square_trustum.h"

#include "azer/math/math.h"
#include "azer/render/render_system.h"
#include "azer/render/util/geometry/util.h"

namespace azer {
SquareTrustum::SquareTrustum(VertexDescPtr desc, float top_length,
                             float bottom_length, float height)
    : GeometryObject(desc),
      top_length_(top_length),
      bottom_length_(bottom_length),
      height_(height) {
  InitHardwareBuffers();
}

SquareTrustum::~SquareTrustum() {
}

void SquareTrustum::InitHardwareBuffers() {
  float top_length = top_length_;
  float bottom_length = bottom_length_;
  float half_height = height_ * 0.5f;
  const Vector4 position[] = {
    Vector4(-top_length,     half_height,     top_length, 1.0f),
    Vector4( top_length,     half_height,     top_length, 1.0f),
    Vector4( bottom_length, -half_height,  bottom_length, 1.0f),
    Vector4(-bottom_length, -half_height,  bottom_length, 1.0f),
    Vector4(-top_length,     half_height,    -top_length, 1.0f),
    Vector4( top_length,     half_height,    -top_length, 1.0f),
    Vector4( bottom_length, -half_height, -bottom_length, 1.0f),
    Vector4(-bottom_length, -half_height, -bottom_length, 1.0f),
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

  VertexPos npos, tpos;
  bool kHasNormal = GetSemanticIndex("normal", 0, desc_.get(), &npos);
  GetSemanticIndex("texcoord", 0, desc_.get(), &tpos);
  SlotVertexDataPtr vdata(new SlotVertexData(desc_, arraysize(indices)));
  VertexPack vpack(vdata.get());
  vpack.first();
  for (int i = 0; i < static_cast<int>(arraysize(indices)); ++i) {
    int index = indices[i];
    DCHECK(!vpack.end());
    vpack.WriteVector4(position[index], VertexPos(0, 0));
    vpack.WriteVector2(texcoord0[index], tpos);
    vpack.next(1);
  }
  DCHECK(vpack.end());

  if (kHasNormal) {
    vpack.first(); 
    for (int i = 0; i < static_cast<int>(arraysize(indices)); i += 6) {
      const Vector4& p1 = position[indices[i]];
      const Vector4& p2 = position[indices[i+1]];
      const Vector4& p3 = position[indices[i+2]];
      Vector4 normal = Vector4(std::move(CalcPlaneNormal(p1, p2, p3)), 0.0);
      for (int j = 0; j < 6; ++j) { 
        vpack.WriteVector4(normal, npos);
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

void SquareTrustum::Render(Renderer* renderer) {
  renderer->Draw(vb_.get(), kTriangleList);
}

void SquareTrustum::RenderWireframe(Renderer* renderer) {
  GeometryObject::RenderWireframe(renderer);
}
}  // namespace azer
