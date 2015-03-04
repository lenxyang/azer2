#pragma once

#include <map>
#include <vector>

#include "azer/base/export.h"
#include "base/basictypes.h"
#include "azer/render/renderable_object.h"

namespace azer {
class Renderer;
class RenderQueue;

class AZER_EXPORT QueueRenderer {
 public:
  QueueRenderer();
  ~QueueRenderer();
  void Render(Renderer* renderer);

  void AddRenderQueue(int32 level, RenderQueuePtr& queue);
  bool RemoveQueue(int32 level, RenderQueuePtr& queue);

  int32 size() const;

  // clear all queues
  void Reset();

  // clear all objects in RenderQueue
  void Clear();
 private:
  typedef std::map<std::string, RenderQueuePtr> LevelQueueMap;
  std::vector<LevelQueue> levels_;
  
  DISALLOW_COPY_AND_ASSIGN(QueueRenderer);
};
}  // namespace azer
