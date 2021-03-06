#include "azer/math/matrix4.h"
#include "azer/math/math.h"
#include "base/logging.h"


namespace azer {
const Matrix4 Matrix4::kIdentity = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                           0.0f, 1.0f, 0.0f, 0.0f,
                                           0.0f, 0.0f, 1.0f, 0.0f,
                                           0.0f, 0.0f, 0.0f, 1.0f);

const Matrix4 Matrix4::kZero = Matrix4(0.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 0.0f);

Matrix4 operator + (const Matrix4& m1, const Matrix4& m2) {
  Matrix4 m(m1.m_[0][0] + m2.m_[0][0],
            m1.m_[0][1] + m2.m_[0][1],
            m1.m_[0][2] + m2.m_[0][2],
            m1.m_[0][3] + m2.m_[0][3],
            m1.m_[1][0] + m2.m_[0][0],
            m1.m_[1][1] + m2.m_[1][1],
            m1.m_[1][2] + m2.m_[1][2],
            m1.m_[1][3] + m2.m_[1][3],
            m1.m_[2][0] + m2.m_[2][0],
            m1.m_[2][1] + m2.m_[2][1],
            m1.m_[2][2] + m2.m_[2][2],
            m1.m_[2][3] + m2.m_[2][3],
            m1.m_[3][0] + m2.m_[3][0],
            m1.m_[3][1] + m2.m_[3][1],
            m1.m_[3][2] + m2.m_[3][2],
            m1.m_[3][3] + m2.m_[3][3]);
  return m;
}

Matrix4 operator - (const Matrix4& m1, const Matrix4& m2) {
  Matrix4 m(m1.m_[0][0] - m2.m_[0][0],
            m1.m_[0][1] - m2.m_[0][1],
            m1.m_[0][2] - m2.m_[0][2],
            m1.m_[0][3] - m2.m_[0][3],
            m1.m_[1][0] - m2.m_[0][0],
            m1.m_[1][1] - m2.m_[1][1],
            m1.m_[1][2] - m2.m_[1][2],
            m1.m_[1][3] - m2.m_[1][3],
            m1.m_[2][0] - m2.m_[2][0],
            m1.m_[2][1] - m2.m_[2][1],
            m1.m_[2][2] - m2.m_[2][2],
            m1.m_[2][3] - m2.m_[2][3],
            m1.m_[3][0] - m2.m_[3][0],
            m1.m_[3][1] - m2.m_[3][1],
            m1.m_[3][2] - m2.m_[3][2],
            m1.m_[3][3] - m2.m_[3][3]);
  return m;
}


Matrix4 Matrix4::InverseCopy() const {
  float m00 = m_[0][0], m01 = m_[0][1], m02 = m_[0][2], m03 = m_[0][3];
  float m10 = m_[1][0], m11 = m_[1][1], m12 = m_[1][2], m13 = m_[1][3];
  float m20 = m_[2][0], m21 = m_[2][1], m22 = m_[2][2], m23 = m_[2][3];
  float m30 = m_[3][0], m31 = m_[3][1], m32 = m_[3][2], m33 = m_[3][3];

  float v0 = m20 * m31 - m21 * m30;
  float v1 = m20 * m32 - m22 * m30;
  float v2 = m20 * m33 - m23 * m30;
  float v3 = m21 * m32 - m22 * m31;
  float v4 = m21 * m33 - m23 * m31;
  float v5 = m22 * m33 - m23 * m32;

  float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
  float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
  float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
  float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

  float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

  float d00 = t00 * invDet;
  float d10 = t10 * invDet;
  float d20 = t20 * invDet;
  float d30 = t30 * invDet;

  float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
  float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
  float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
  float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

  v0 = m10 * m31 - m11 * m30;
  v1 = m10 * m32 - m12 * m30;
  v2 = m10 * m33 - m13 * m30;
  v3 = m11 * m32 - m12 * m31;
  v4 = m11 * m33 - m13 * m31;
  v5 = m12 * m33 - m13 * m32;

  float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
  float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
  float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
  float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

  v0 = m21 * m10 - m20 * m11;
  v1 = m22 * m10 - m20 * m12;
  v2 = m23 * m10 - m20 * m13;
  v3 = m22 * m11 - m21 * m12;
  v4 = m23 * m11 - m21 * m13;
  v5 = m23 * m12 - m22 * m13;

  float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
  float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
  float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
  float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

  return Matrix4(
      d00, d01, d02, d03,
      d10, d11, d12, d13,
      d20, d21, d22, d23,
      d30, d31, d32, d33);
}

Matrix4 Matrix4::InverseAffineCopy() const {
  DCHECK(IsAffine());

  float m10 = m_[1][0], m11 = m_[1][1], m12 = m_[1][2];
  float m20 = m_[2][0], m21 = m_[2][1], m22 = m_[2][2];

  float t00 = m22 * m11 - m21 * m12;
  float t10 = m20 * m12 - m22 * m10;
  float t20 = m21 * m10 - m20 * m11;

  float m00 = m_[0][0], m01 = m_[0][1], m02 = m_[0][2];

  float invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

  t00 *= invDet; t10 *= invDet; t20 *= invDet;

  m00 *= invDet; m01 *= invDet; m02 *= invDet;

  float r00 = t00;
  float r01 = m02 * m21 - m01 * m22;
  float r02 = m01 * m12 - m02 * m11;

  float r10 = t10;
  float r11 = m00 * m22 - m02 * m20;
  float r12 = m02 * m10 - m00 * m12;

  float r20 = t20;
  float r21 = m01 * m20 - m00 * m21;
  float r22 = m00 * m11 - m01 * m10;

  float m03 = m_[0][3], m13 = m_[1][3], m23 = m_[2][3];

  float r03 = - (r00 * m03 + r01 * m13 + r02 * m23);
  float r13 = - (r10 * m03 + r11 * m13 + r12 * m23);
  float r23 = - (r20 * m03 + r21 * m13 + r22 * m23);

  return Matrix4(
      r00, r01, r02, r03,
      r10, r11, r12, r13,
      r20, r21, r22, r23,
      0,   0,   0,   1);
}
}  // namespace azer
