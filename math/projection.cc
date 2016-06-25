#include "azer/math/matrix4.h"
#include "azer/math/projection.h"
#include "base/logging.h"


namespace azer {

Matrix4 PerspectiveRH(Radians fovy, float aspect, float zn, float zf) {
  DCHECK_GT(aspect, 1.0f);
  Matrix4 mat = Matrix4::kIdentity;
  const float tan_half_fov = tan(fovy / 2.0f);
  const float z_range = (zf - zn);
  float yscale = 1.0f / tan_half_fov;
  float xscale = yscale / aspect;

  mat[0][0] = xscale;
  mat[1][1] = yscale;
  mat[2][2] = -(zf + zn) / z_range;
  mat[2][3] = -2.0f * zf * zn / z_range;
  
  mat[3][2] = -1.0f;
  mat[3][3] = 0.0f;
  return mat;
}

Matrix4 PerspectiveRHD3D(Radians fovy, float aspect, float zn, float zf) {
  const float tan_half_fov = tan(fovy / 2.0f);
  const float z_range = (zf - zn);
  float yscale = 1 / tan_half_fov;
  float xscale = yscale / aspect;

  return Matrix4(xscale,  0.0f,           0.0f,                   0.0f,
                 0.0f,  yscale,           0.0f,                   0.0f,
                 0.0f,    0.0f,   zf / (zn - zf),  zn * zf / (zn - zf),
                 0.0f,    0.0f,          -1.0f,                  0.0f);
}

Matrix4 OrthoProjRH(float width, float height, float zn, float zf) {
  Matrix4 mat = Matrix4::kIdentity;
  const float z_range = (zn - zf);
  return Matrix4(0.5f * width, 0.0f,          0.0f,              0.0f,
                 0.0f,         0.5f * height, 0.0f,              0.0f,
                 0.0f,         0.0f,          1.0f / z_range,    zn / z_range,
                 0.0f,         0.0f,          0.0f,              1.0f);
}
}  // namespace azer
