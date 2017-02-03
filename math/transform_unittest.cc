#include "azer/math/math.h"
#include "gtest/gtest.h"
#include "azer/math/glm_util.h"
#include "glm/gtx/transform.hpp"

namespace azer {
TEST(Transform, Perspective) {
  float aspect = 4.0f / 3.0f;
  Matrix4 proj = Transform::PerspectiveRH(Degree(60.0f), aspect, 1.0, 100.0f);
  glm::mat4 glm_proj = glm::perspective(60.0f, aspect, 1.0f, 100.0f);
  ASSERT_TRUE(proj == glm_proj);
}

TEST(Transform, LookAtLH) {
}

TEST(Transform, LookAtRH) {
  {
    Vector3 pos(0, 0, -1);
    Vector3 lookat(0, 0, 0);
    Vector3 up(0, 1, 0);
    Matrix4 view_rh = Transform::LookAtRH(pos, lookat, up);
    glm::mat4 glm_mat = glm::lookAt(glm_conv(pos), glm_conv(lookat), glm_conv(up));
    ASSERT_TRUE(view_rh == glm_mat);
  }

  {
    Vector3 pos(0, 0, 1);
    Vector3 lookat(0, 0, -1);
    Vector3 up(0, 1, 0);
    Matrix4 view_rh = Transform::LookAtRH(pos, lookat, up);
    glm::mat4 glm_mat = glm::lookAt(glm_conv(pos), glm_conv(lookat), glm_conv(up));
    ASSERT_TRUE(view_rh == glm_mat);
  }

  {
    Vector3 pos(0.0f, 10.0f, 18.0f);
    Vector3 lookat(0.0f, 0.0f, -20.0f);
    Vector3 up(0.0f, 1.0f, 0.0f);
    Matrix4 view_rh = Transform::LookAtRH(pos, lookat, up);
    glm::mat4 glm_mat = glm::lookAt(glm_conv(pos), glm_conv(lookat), glm_conv(up));
    ASSERT_TRUE(view_rh == glm_mat);
  }

  {
    Vector3 pos(12.0f, 8.0f, 0.0f);
    Vector3 lookat(0.0f, 0.0f, 0.0f);
    Vector3 up(0.0f, 1.0f, 0.0f);
    Matrix4 view_rh = Transform::LookAtRH(pos, lookat, up);
    glm::mat4 glm_mat = glm::lookAt(glm_conv(pos), glm_conv(lookat), glm_conv(up));
    ASSERT_TRUE(view_rh == glm_mat);
  }

  {
    Vector3 pos(-14.127034f, 3.5436306f, -0.13424602f);
    Vector3 dir(0.99847746f, -0.017452411f, 0.052327991f);
    Vector3 up(0.017428493f, 0.99984771f, 0.00091339089f);
    Vector3 lookat = dir + pos;
    Matrix4 view_dir_rh = LookDirRH(pos, dir, up);
    Matrix4 view_at_rh = Transform::LookAtRH(pos, lookat, up);
    glm::mat4 glm_mat = glm::lookAt(glm_conv(pos), glm_conv(lookat), glm_conv(up));
    ASSERT_TRUE(view_dir_rh == glm_mat);
    ASSERT_TRUE(view_at_rh == view_at_rh);
  }

  {
    Vector3 pos(-16.544796f, 8.0865746f, -0.26095578f);
    Vector3 dir(0.94422287f, -0.32556826f, 0.049484849f);
    Vector3 up(0.017428493f, 0.99984771f, 0.00091339089f);
    Vector3 lookat = dir + pos;
    Matrix4 view_dir_rh = LookDirRH(pos, dir, up);
    Matrix4 view_at_rh = Transform::LookAtRH(pos, lookat, up);
    glm::mat4 glm_mat = glm::lookAt(glm_conv(pos), glm_conv(lookat), glm_conv(up));
    ASSERT_TRUE(view_dir_rh == glm_mat);
    ASSERT_TRUE(view_at_rh == view_at_rh);
  }
}
}  // namespace azer
