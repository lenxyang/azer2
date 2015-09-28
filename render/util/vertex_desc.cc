#include "azer/render/util/vertex_desc.h"

#include "azer/render/render_system.h"

namespace azer {

// class PositionVertex
const VertexDesc::Desc PositionVertex::kVertexDesc[] = {
  {"POSITION", 0, kVec4},
};

const int PositionVertex::kVertexDescNum = arraysize(kVertexDesc);
VertexDescPtr PositionVertex::CreateVertexDesc() {
  return VertexDescPtr(new VertexDesc(kVertexDesc, kVertexDescNum));
}

// class PosNormalVertex
const VertexDesc::Desc PosNormalVertex::kVertexDesc[] = {
  {"POSITION", 0, kVec4},
  {"NORMAL", 0, kVec4},
};

const int PosNormalVertex::kVertexDescNum = arraysize(kVertexDesc);
VertexDescPtr PosNormalVertex::CreateVertexDesc() {
  return VertexDescPtr(new VertexDesc(kVertexDesc, kVertexDescNum));
}
}  // namespace azer
