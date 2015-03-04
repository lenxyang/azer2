#include "azer/render_queue/render_queue.h"

#include "base/logging.h"

namespace azer {
RenderQueue::RenderQueue() {
}

RenderQueue::~RenderQueue() {
}

void RenderQueue::AddObject(const RenderableObjectPtr& object) {
  DCHECK(!Exists(object));
  queue_.push_back(object);
}

bool RenderQueue::RemoveObject(const RenderableObjectPtr& object) {
  auto iter = std::find(children_.begin(), children_.end(), object);
  if (iter != children_.end()) {
    queue_.erase(iter);
    return true;
  } else {
    return false;
  }
}

bool RenderQueue::Exists(const RenderableObjectPtr& object) {
  return queue_.end() != std::find(children_.begin(), children_.end(), object);
}


}  // namespace azer
