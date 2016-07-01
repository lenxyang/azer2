#pragma once

#include "azer/render/entity.h"

namespace azer {
class Camera;
class VertexDesc;
EntityDataPtr CreateFrustumBox(VertexDesc* desc, const Camera& camera);
EntityDataPtr CreateFrustumBoxFrame(VertexDesc* desc, const Camera& camera);
}  // namespace azer
