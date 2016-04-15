#pragma once

#include <utility>
#include "azer/math/degree.h"

namespace azer {


// https://msdn.microsoft.com/en-us/library/windows/desktop/bb206341(v=vs.85).aspx
// FOV: represent the viewport's horizontal and vertical fields of view, in radians.
// Aspect: viewport aspect ratio 

/**
 * 右手坐标系
 */

Matrix4 PerspectiveRH(Radians fovy, float aspect, float near, float far);
Matrix4 PerspectiveRH(Degree fovy, float aspect, float near, float far);

/**
 * [-1.0f, -1.0f, -1.0f] ~ [1.0f, 1.0f, 0.0f]
 */
Matrix4 PerspectiveRHD3D(Radians fovy, float aspect, float near, float far);
Matrix4 PerspectiveRHD3D(Degree fovy, float aspect, float near, float far);

inline Matrix4 PerspectiveRH(Degree fovy, float aspect, float z_near, float z_far) {
  return std::move(PerspectiveRH(Radians(fovy), aspect, z_near, z_far));
}

inline Matrix4 PerspectiveRHD3D(Degree fovy, float aspect, float z_near, float z_far) {
  return std::move(PerspectiveRHD3D(Radians(fovy), aspect, z_near, z_far));
}

Matrix4 OrthoProjRH(float width, float height, float z_near, float z_far);
}  // namespace azer
