#include "azer/math/transform.h"

#include "azer/math/math.h"

namespace azer {

Matrix4 Transform::Translate(float x, float y, float z) {
  Matrix4 mat = Matrix4::kIdentity;
  mat[0][3] = x;
  mat[1][3] = y;
  mat[2][3] = z;
  return mat;
}

Matrix4 Transform::Translate(const Vector3& pos) {
  return Translate(pos.x, pos.y, pos.z);
}

Matrix4 Transform::Scale(float scale) {
  return Matrix4(scale, 0.0f,  0.0f,  0.0f,
                 0.0f,  scale, 0.0f,  0.0f,
                 0.0f,  0.0f,  scale, 0.0f,
                 0.0f,  0.0f,  0.0f,  1.0f);
}

Matrix4 Transform::Scale(float x, float y, float z) {
  return Matrix4(x,     0.0f,  0.0f,  0.0f,
                 0.0f,  y,     0.0f,  0.0f,
                 0.0f,  0.0f,  z,     0.0f,
                 0.0f,  0.0f,  0.0f,  1.0f);
}

Matrix4 Transform::Scale(const Vector3& scale) {
  return Matrix4(scale.x,  0.0f,      0.0f,        0.0f,
                 0.0f,       scale.y, 0.0f,        0.0f,
                 0.0f,       0.0f,      scale.z,   0.0f,
                 0.0f,       0.0f,      0.0f,      1.0f);
}

Matrix4 Transform::RotateX(Degree d) { return RotateX(Radians(d));}
Matrix4 Transform::RotateX(Radians r) {
  float s = sin(r);
  float c = cos(r);
  return Matrix4(1.0f,  0.0f,  0.0f,  0.0f,
                 0.0f,     c,     s,  0.0f,
                 0.0f,    -s,     c,  0.0f,
                 0.0f,  0.0f,  0.0f,  1.0f);
}

Matrix4 Transform::RotateY(Degree d) { return RotateY(Radians(d));}
Matrix4 Transform::RotateY(Radians r) {
  float s = sin(r);
  float c = cos(r);
  return Matrix4(c,     0.0f,    -s,  0.0f,
                 0.0f,  1.0f,  0.0f,  0.0f,
                 s,    0.0f,      c,  0.0f,
                 0.0f,  0.0f,  0.0f,  1.0f);
}

Matrix4 Transform::RotateZ(Degree d) { return RotateZ(Radians(d));}
Matrix4 Transform::RotateZ(Radians r) {
  float s = sin(r);
  float c = cos(r);
  return Matrix4(   c,    s,  0.0f,  0.0f,
                    -s,    c,  0.0f,  0.0f,
                    0.0f,  0.0f,  1.0f,  0.0f,
                    0.0f,  0.0f,  0.0f,  1.0f);
}

Matrix4 Transform::LookAtRH(const Vector3& eye, const Vector3& lookat,
                            const Vector3& up) {
  Vector3 camera_z = (eye - lookat).NormalizeCopy();
  return LookDirRH(eye, -camera_z, up);
}

Matrix4 Transform::PerspectiveRH(Degree fovy, float aspect, 
                                 float z_near, float z_far) {
  return std::move(PerspectiveRH(Radians(fovy), aspect, z_near, z_far));
}

/*
 *  右手坐标系的 LookAt 矩阵
 *  需要非常注意的是，右手坐标系矩阵的 Z 轴与可视方向是相反的
 *  即 z 轴的方向是 position - target 
 */
// right handle
// zaxis = normal(cameraPosition - cameraTarget)
// xaxis = normal(cross(cameraUpVector, zaxis))
// yaxis = cross(zaxis, xaxis)
// xaxis.x           yaxis.x           zaxis.x          -dot(xaxis, cameraPosition)
// xaxis.y           yaxis.y           zaxis.y          -dot(yaxis, cameraPosition)
// xaxis.z           yaxis.z           zaxis.z          -dot(zaxis, cameraPosition)
// 0                       0                 0                                    1

Matrix4 Transform::LookDirRH(const Vector3& eye, const Vector3& dir,
                             const Vector3& up) {
  Vector3 camera_z = -dir.NormalizeCopy();
  Vector3 up_normed = std::move(up.NormalizeCopy());
  Vector3 camera_x = std::move(up_normed.cross(camera_z).NormalizeCopy());
  Vector3 camera_y = std::move(camera_z.cross(camera_x).NormalizeCopy());

  Matrix4 mat = Matrix4::kIdentity;
  mat[0][0] = camera_x.x;
  mat[0][1] = camera_x.y;
  mat[0][2] = camera_x.z;
  
  mat[1][0] = camera_y.x;
  mat[1][1] = camera_y.y;
  mat[1][2] = camera_y.z;
  
  mat[2][0] = camera_z.x;
  mat[2][1] = camera_z.y;
  mat[2][2] = camera_z.z;
  
  mat[0][3] = -camera_x.dot(eye);
  mat[1][3] = -camera_y.dot(eye);
  mat[2][3] = -camera_z.dot(eye);
  return mat;
}

/*
 *  左手坐标系的 LookAt 矩阵
 *  需要非常注意的是，右手坐标系矩阵的 Z 轴与可视方向是相反的
 *  即 z 轴的方向是 position - target 
 */
// zaxis = normal(cameraTarget - cameraPosition)
// xaxis = normal(cross(cameraUpVector, zaxis))
// yaxis = cross(zaxis, xaxis)
// xaxis.x           yaxis.x           zaxis.x          -dot(xaxis, cameraPosition)
// xaxis.y           yaxis.y           zaxis.y          -dot(yaxis, cameraPosition)
// xaxis.z           yaxis.z           zaxis.z          -dot(zaxis, cameraPosition)
// 0                       0                 0                                    1
/*
  Matrix4 Transform::LookAtLH(const Vector3& eye, const Vector3& lookat, 
  const Vector3& up) {
  Vector3 camera_z = (lookat - eye).NormalizeCopy();
  Vector3 up_normed = std::move(up.NormalizeCopy());
  Vector3 camera_x = std::move(up_normed.cross(camera_z).NormalizeCopy());
  Vector3 camera_y = std::move(camera_z.cross(camera_x).NormalizeCopy());

  Matrix4 mat = Matrix4::kIdentity;
  mat[0][0] = camera_x.x;
  mat[0][1] = camera_x.y;
  mat[0][2] = camera_x.z;
  
  mat[1][0] = camera_y.x;
  mat[1][1] = camera_y.y;
  mat[1][2] = camera_y.z;
  
  mat[2][0] = camera_z.x;
  mat[2][1] = camera_z.y;
  mat[2][2] = camera_z.z;
  
  mat[0][3] = -camera_x.dot(eye);
  mat[1][3] = -camera_y.dot(eye);
  mat[2][3] = -camera_z.dot(eye);
  return mat;
  }
*/

#define SQR(x) ((x) * (x))

Matrix4 Transform::Reflect(const Plane& plane) {
  // reference http://en.wikipedia.org/wiki/Transformation_matrix
  Matrix4 mat = Matrix4::kIdentity;
  mat[0][0] = 1 - 2 * SQR(plane.normal().x);
  mat[0][1] = -2 * plane.normal().x * plane.normal().y;
  mat[0][2] = -2 * plane.normal().x * plane.normal().z;
  mat[0][3] = -2 * plane.normal().x * plane.d();

  mat[1][0] = -2 * plane.normal().x * plane.normal().y;
  mat[1][1] = 1 - 2 * SQR(plane.normal().y);
  mat[1][2] = -2 * plane.normal().y * plane.normal().z;
  mat[1][3] = -2 * plane.normal().y * plane.d();

  mat[2][0] = -2 * plane.normal().x * plane.normal().z;
  mat[2][1] = -2 * plane.normal().y * plane.normal().z;
  mat[2][2] = 1 - 2 * SQR(plane.normal().z);
  mat[2][3] = -2 * plane.normal().z * plane.d();
  return mat;
}

Matrix4 Transform::PerspectiveRH(Radians fovy, float aspect, float zn, float zf) {
  const float tan_half_fov = tan(fovy / 2.0f);
  const float z_range = (zf - zn);
  float yscale = 1 / tan_half_fov;
  float xscale = yscale / aspect;

  return Matrix4(xscale,  0.0f,           0.0f,                   0.0f,
                 0.0f,  yscale,           0.0f,                   0.0f,
                 0.0f,    0.0f,   zf / (zn - zf),  zn * zf / (zn - zf),
                 0.0f,    0.0f,          -1.0f,                  0.0f);
}

Matrix4 Transform::OrthoProjRH(float width, float height, float zn, float zf) {
  const float z_range = (zn - zf);
  return Matrix4(2.0f / width, 0.0f,          0.0f,              0.0f,
                 0.0f,         2.0f / height, 0.0f,              0.0f,
                 0.0f,         0.0f,          1.0f / z_range,    zn / z_range,
                 0.0f,         0.0f,          0.0f,              1.0f);
}

Matrix4 Transform::OrthoOffCenterRH(float l, float r, float b, float t, 
                                    float zn, float zf) {
  const float z_range = (zn - zf);
  return Matrix4(2.0f / (r - 1.0f), 0.0f,              0.0f,            0.0f, 
                 0.0f,              2.0f / (t - b),    0.0f,            0.0f,
                 0.0f,              0.0f,              1.0f / z_range,  0.0f,
                 (1 + r) / (1 - r), (t + b) / (b - t), zn / z_range,    1.0f);
}
}  // namespace azer
