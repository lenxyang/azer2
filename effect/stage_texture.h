#pragma once

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"

namespace azer {

class Texture;
class Renderer;
typedef scoped_refptr<Texture> TexturePtr;

struct AZER_EXPORT StageTexture {
  int stage;
  int count;
  int mode;
  TexturePtr tex[32];

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
