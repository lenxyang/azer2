#pragma once

#include "base/basictypes.h"
#include "azer/util/vertex_pack.h"
#include "azer/util/index_pack.h"

namespace azer { 
struct Pitch {
  int left;
  int right;
  int top;
  int bottom;

  Pitch(): left(-1), right(-1), top(-1), bottom(-1) {}
  Pitch(int l, int t, int r, int b): left(l), right(r), top(t), bottom(b) {}
  Pitch(const Pitch& pitch) {*this = pitch; }
  Pitch& operator = (const Pitch& pitch) {
    left = pitch.left;
    right = pitch.right;
    top = pitch.top;
    bottom = pitch.bottom;
    return *this;
  }
};

class Tile {
 public:
  Tile(int32 level);
  int32 GetVertexCount() const;
  int32 GetTriangleListIndexCount() const;
  void GenerateTriangleListIndices(int32 step, const Pitch& pitch, IndexPack* pack);
  void GenerateTriangleListIndices(int32 step, IndexPack* pack);

  int32 index(int x, int z) const;
  int32 grid_line() const { return kGridLine;}
  int32 cell_count() const { return grid_line() - 1;}
  int32 level() const { return kLevel;}
 private:
  const int32 kLevel;
  const int32 kGridLine;
  DISALLOW_COPY_AND_ASSIGN(Tile);
};

int32 CalcTileLevel(uint32 gridline);
}  // namespace azer
