#include "azer/render/gpu_constants_table.h"
#include "azer/math/math.h"

#include "base/logging.h"
#include "gtest/gtest.h"

using namespace azer;

class DummyShaderParamTable : azer::ShaderParamTable {
 public:
  virtual void flush() {
  }
};

TEST(ShaderParamTable, SetValue) {
}

TEST(ShaderParamTable, SetLight) {
}

