#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/effect.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/transform_holder.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/technique.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/util/coordinate_grid_effect.h"

namespace azer {

class AZER_EXPORT CoordinateGrid {
 public:
  CoordinateGrid(float width, float height, int num);
  ~CoordinateGrid();

  void Update(const Camera& camera);
  void Render(Renderer* rs);
  void SetXCoordColor(const Vector4& col) { x_color_ = col;}
  void SetZCoordColor(const Vector4& col) { z_color_ = col;}

  TransformHolder* GetTransformHolder() { return &holder_;}
 private:
  void Init();

  scoped_refptr<CoordinateGridEffect> effect_ptr_;
  VertexDataPtr data_;
  VertexBufferPtr vb_;
  Vector4 x_color_, z_color_;
  TransformHolder holder_;
  const float kWidth, kHeight;
  const int kNum;
  DISALLOW_COPY_AND_ASSIGN(CoordinateGrid);
};
}  // namespace azer
