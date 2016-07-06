#include "azer/util/geometry/frustum.h"

#include "azer/math/math.h"
#include "azer/render/camera.h"
#include "azer/util/geometry/box.h"

namespace azer {
EntityDataPtr CreateFrustumBox(VertexDesc* desc, const Camera& camera) {
  Vector3 pos[8];
  CalcCameraBundingPos(camera, pos);
  Vector4 posvec[8];
  for (int i = 0; i < arraysize(pos); ++i) {
    posvec[i] = Vector4(pos[i], 1.0f);
  }

  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoHexaHedronFrameData(data.get(), posvec, Matrix4::kIdentity);
  AppendGeoHexaHedronData(data.get(), posvec, Matrix4::kIdentity);
  return data;
}
}  // namespace azer
