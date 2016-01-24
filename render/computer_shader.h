#pragma once

#include <vector>

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
class Texture;

class AZER_EXPORT ComputerShader {
 public:
  ComputerShader() {}
  virtual ~ComputerShader() {}

  virtual void SetInputTexture(int32 index, Texture* tex);
  virtual void SetOutputTexture(int32 index, Texture* tex);
  virtual void Dispatch(); 
 private:
  DISALLOW_COPY_AND_ASSIGN(ComputerShader);
};
}  // namespace azer
