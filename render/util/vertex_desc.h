#pragma once

#include "azer/math/math.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

struct PositionVertex {
 public:
  Vector4 position;
  static VertexDescPtr CreateVertexDesc();
 private:
  static const int kVertexDescNum;
  static const VertexDesc::Desc kVertexDesc[];
};

struct PosNormalVertex {
 public:
  Vector4 position;
  Vector4 normal;
  static VertexDescPtr CreateVertexDesc();
 private:
  static const int kVertexDescNum;
  static const VertexDesc::Desc kVertexDesc[];
};
}  // namespace azer
