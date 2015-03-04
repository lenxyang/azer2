#include "azer/render_queue/render_queue.h"

#include "base/logging.h"

namespace azer {
RenderQueue::RenderQueue() {
}

RenderQueue::~RenderQueue() {
}

void RenderQueue::AddObject(const RenderableObjectPtr& object) {
  queue_.push_back(object);
}

bool RenderQueue::RemoveObject(const RenderableObjectPtr& object) {
  return true;
}

bool RenderQueue::Exists(const RenderableObjectPtr& object) {
  return false;
}


}  // namespace azer
