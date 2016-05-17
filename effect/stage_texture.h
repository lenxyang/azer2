#pragma once

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "azer/render/texture_view.h"

namespace azer {

class TextureView;
class Renderer;
typedef scoped_refptr<TextureView> TextureViewPtr;

struct AZER_EXPORT StageTexture {
  int stage;
  int count;
  int mode;
  TextureViewPtr tex[32];

  StageTexture();
};

class AZER_EXPORT StageTexContainer {
 public:
  StageTexContainer();
  ~StageTexContainer();

  void SetTex(int stage, int count, TextureView* tex);
  void Bind(Renderer* renderer);
  void Reset(Renderer* renderer);
 private:
  StageTexture& GetStage(int32 stage);
  std::vector<StageTexture> stages_;
  DISALLOW_COPY_AND_ASSIGN(StageTexContainer);
};

}  // namespace azer
