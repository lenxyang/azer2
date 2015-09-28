#include "wow/render/vertex_desc.h"

#include "azer/render/render_system.h"

namespace azer {

const VertexDesc::Desc PositionVertex::kVertexDesc[] = {
  {"POSITION", 0, kVec4},
};

const int PositionVertex::kVertexDescNum = arraysize(kVertexDesc);
VertexDescPtr PositionVertex::CreateVertexDesc() {
  return VertexDescPtr(new VertexDesc(kVertexDesc, kVertexDescNum));
}
}  // namespace azer
