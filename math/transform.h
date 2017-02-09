#pragma once

namespace azer {
class Plane;
class Degree;
class Radians;
class Matrix4;
class Vector3;
class Vector4;

class Transform {
 public:
  static Matrix4 Translate(float x, float y, float z);
  static Matrix4 Translate(const Vector3& pos);

  static Matrix4 RotateX(Radians r);
  static Matrix4 RotateX(Degree r);

  static Matrix4 RotateY(Radians r);
  static Matrix4 RotateY(Degree r);

  static Matrix4 RotateZ(Radians r);
  static Matrix4 RotateZ(Degree r);

  static Matrix4 Scale(float scale);
  static Matrix4 Scale(float x, float y, float z);
  static Matrix4 Scale(const Vector3& scale);


  /**
   * eye: position of camera
   * lookat: position of camera look at
   * up: direction of camear up
   * Note: lookat is a vector not a position
   */
  static Matrix4 LookAtRH(const Vector3& eye, const Vector3& lookat,
                          const Vector3& up);
  /**
   * 注意： dir 为 camera 对准的方向
   * 在右手坐标系当中，它与 Z 轴的方向是相反的
   */
  static Matrix4 LookDirRH(const Vector3& eye, const Vector3& dir,
                           const Vector3& up);
  static Matrix4 Reflect(const Plane& plane);

  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb206341(v=vs.85).aspx
  // FOV: represent the viewport's horizontal and vertical fields of view, 
  // in radians. 
  // Aspect: viewport aspect ratio 
  // NDC: [-1.0f, -1.0f, -1.0f] ~ [1.0f, 1.0f, 0.0f]
  static Matrix4 PerspectiveRH(Radians fovy, float aspect, float near, float far);
  static Matrix4 PerspectiveRH(Degree fovy, float aspect, float near, float far);
  static Matrix4 OrthoProjRH(float width, float height, float z_near, float z_far);
  static Matrix4 OrthoOffCenterRH(float left, float right, float bottom, float top, 
                                  float near, float far);
 private:
  Transform() {}
};
}  // namespace azer
