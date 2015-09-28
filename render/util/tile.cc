#include "azer/util/tile.h"

#include "base/logging.h"
#include "azer/util/index_pack.h"

namespace azer {

Tile::Tile(int32 level) 
    : kGridLine( (1 << level) + 1),
      kLevel(level) {
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
