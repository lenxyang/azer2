#include "azer/render/render_queue_cluster.h"

namespace azer {

void RenderQueueCluster::Clear() {
  for (uint32 index = 0; index < levels_.size(); ++index) { 
    for (auto iter = levels_[index].begin();
         iter != levels_[index].end();
         ++iter) {
      iter->second->Clear();
    }
  }
}

void RenderQueueCluster::Reset() {
  levels_.clear();
}

void RenderQueueCluster::Render(const FrameArgs& frame, Renderer* renderer) {
  for (uint32 index = 0; index < levels_.size(); ++index) { 
    for (auto iter = levels_[index].begin();
         iter != levels_[index].end();
         ++iter) {
      iter->second->Render(frame, renderer);
    }
  }
}

void RenderQueueCluster::AddRenderQueue(int32 level, RenderQueuePtr& queue) {
}

bool RenderQueueCluster::RemoveQueue(int32 level, RenderQueuePtr& queue) {
  return true;
}

}  // namespace azer
