#pragma once

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "azer/render/resource_view.h"

namespace azer {

class Renderer;

struct AZER_EXPORT StageResource {
  int stage;
  int count;
  int mode;
  ResourceViewPtr tex[32];

  StageResource();
};

class AZER_EXPORT StageResContainer {
 public:
  StageResContainer();
  ~StageResContainer();

  void SetResource(int stage, int count, ResourceView* tex);
  void Bind(Renderer* renderer);
  void Reset(Renderer* renderer);
 private:
  StageResource& GetStage(int32_t stage);
  std::vector<StageResource> stages_;
  DISALLOW_COPY_AND_ASSIGN(StageResContainer);
};

}  // namespace azer
