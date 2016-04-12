#pragma once

#include "azer/base/export.h"

namespace azer {

class Texture;
class Renderer;

struct AZER_EXPORT StageTexture {
  int stage;
  int count;
  int mode;
  Texture* tex[32];

  StageTexture();
};

class AZER_EXPORT StageTexContainer {
 public:
  StageTexContainer();
  ~StageTexContainer();

  void SetTex(int stage, int count, Texture* tex);
  void Bind(Renderer* renderer);
  void Reset(Renderer* renderer);
 private:
  StageTexture& GetStage(int32 stage);
  std::vector<StageTexture> stages_;
  DISALLOW_COPY_AND_ASSIGN(StageTexContainer);
};

}  // namespace azer
