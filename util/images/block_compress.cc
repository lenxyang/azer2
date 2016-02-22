#include "azer/util/images/block_compress.h"

namespace azer {
namespace {
void Decode565(const uint16_t w565, HDRColorA *col) {
  col->r = (float) ((w565 >> 11) & 31) * (1.0f / 31.0f);
  col->g = (float) ((w565 >>  5) & 63) * (1.0f / 63.0f);
  col->b = (float) ((w565 >>  0) & 31) * (1.0f / 31.0f);
  col->a = 1.0f;
}

uint16_t Encode565(_In_ const HDRColorA *col) {
  HDRColorA Color;

  Color.r = (col->r < 0.0f) ? 0.0f : (col->r > 1.0f) ? 1.0f : col->r;
  Color.g = (col->g < 0.0f) ? 0.0f : (col->g > 1.0f) ? 1.0f : col->g;
  Color.b = (col->b < 0.0f) ? 0.0f : (col->b > 1.0f) ? 1.0f : col->b;

  uint16_t w;

  w = (uint16_t) ((static_cast<int32_t>(Color.r * 31.0f + 0.5f) << 11) |
                  (static_cast<int32_t>(Color.g * 63.0f + 0.5f) <<  5) |
                  (static_cast<int32_t>(Color.b * 31.0f + 0.5f) <<  0));

  return w;
}

}
}  // namespace azer
