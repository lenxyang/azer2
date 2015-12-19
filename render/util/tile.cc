#include "azer/render/util/tile.h"

#include "base/logging.h"
#include "azer/render/index_pack.h"
#include "azer/render/vertex_pack.h"

namespace azer {

Tile::Tile(int32 level) 
    : kGridLine( (1 << level) + 1),
      kLevel(level) {
}

void Tile::GenerateVertex(float width, float depth, VertexPack* pack) {
  VertexPos pos_pos;
  CHECK(GetSemanticIndex("position", 0, pack->desc(), &pos_pos));
  float cell_width = width / kGridLine;
  float cell_depth = width / kGridLine;
  
  pack->first();
  for (int32 i = 0; i < kGridLine; ++i) {
    float z = -0.5f * depth + i * cell_depth;
    for (int32 j = 0; j < kGridLine; ++j) {
      float x = -0.5f * width + j * cell_width;
      Vector4 v(x, 0.0f, z, 1.0f);
      pack->WriteVector3Or4(v, pos_pos);
      pack->next(1);
    }
  }
}

int32 Tile::GetVertexCount() const {
  return kGridLine * kGridLine;
}

int32 Tile::GetTriangleListIndexCount() const {
  return (kGridLine - 1) * (kGridLine - 1) * 6;
}

int32 Tile::index(int x, int z) const {
  return z * kGridLine + x;
}

void Tile::GenerateTriangleListIndices(int32 step, IndexPack* pack) {
  Pitch pitch(0, 0, kGridLine, kGridLine);
  GenerateTriangleListIndices(step, pitch, pack);
}

void Tile::GenerateTriangleListIndices(int32 step, const Pitch& pitch, 
                                       IndexPack* pack) {
  for (int i = pitch.top; i < pitch.bottom - 1; i += step) {
    for (int j = pitch.left; j < pitch.right - 1; j += step) {
      int cur_line = i * kGridLine;
      int next_line = (i + step) * kGridLine;
      CHECK(pack->WriteAndAdvance(cur_line  + j));
      CHECK(pack->WriteAndAdvance(next_line + j));
      CHECK(pack->WriteAndAdvance(next_line + j + step));
      CHECK(pack->WriteAndAdvance(cur_line  + j));
      CHECK(pack->WriteAndAdvance(next_line + j + step));
      CHECK(pack->WriteAndAdvance(cur_line  + j + step));
    }
  }
}

int32 CalcTileLevel(uint32 gridline) {
  DCHECK(gridline > 0);
  DCHECK(gridline & 0x1);
  int level = 0;
  int32 mask = 0;
  gridline >>= 1;
  while (gridline > 0) {
    mask = (gridline & 0x1 ? mask + 1 : mask);
    gridline >>= 1;
    level++;
  }
  DCHECK(mask == 1);
  return level;
}
}  // namespace azer
