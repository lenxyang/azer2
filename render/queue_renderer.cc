#include "azer/render_queue/queue_renderer.h"

namespace azer {

void QueueRenderer::Clear() {
  for (uint32 index = 0; index < levels_.size(); ++index) { 
    for (auto iter = levels_[index]->begin();
         iter != levels_[index]->end();
         ++iter) {
      (*iter)->Clear();
    }
  }
}

void QueueRenderer::Reset() {
  levels_.clear();
}

void QueueRenderer::Render(Renderer* renderer) {
  for (uint32 index = 0; index < levels_.size(); ++index) { 
    for (auto iter = levels_[index]->begin();
         iter != levels_[index]->end();
         ++iter) {
      (*iter)->Render(renderer);
    }
  }
}

void QueueRenderer::AddRenderQueue(int32 level, RenderQueuePtr& queue) {
}

bool QueueRenderer::RemoveQueue(int32 level, RenderQueuePtr& queue) {
  return true;
}

}  // namespace azer
