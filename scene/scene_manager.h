#pragma once

#include "azer/base/export.h"
#include "azer/scene/node.h"

namespace azer {
class SceneManager {
 public:
  SceneManager();
  ~SceneManager();
 private:
  SceneNodePtr root_;
  DISALLOW_COPY_AND_ASSIGN(SceneManager);
};
}  // namespace azer
