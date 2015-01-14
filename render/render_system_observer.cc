#include "azer/render/render_system_observer.h"

#include "base/lazy_instance.h"


namespace azer {
namespace subtle {

RenderSystemObserverManager* RenderSystemObserverManager::GetInstance() {
  return NULL;
}

RenderSystemObserverManager::RenderSystemObserverManager() {
}

RenderSystemObserverManager::~RenderSystemObserverManager() {
}

void RenderSystemObserverManager::CallCreated(RenderSystem* rs) {
  FOR_EACH_OBSERVER(RenderSystemObserver,
                    observers_,
                    OnCreated(rs));
}

void RenderSystemObserverManager::CallDestroying(RenderSystem* rs) {
  FOR_EACH_OBSERVER(RenderSystemObserver,
                    observers_,
                    OnDestroying(rs));
}

void RenderSystemObserverManager::CallDestroyed() {
  FOR_EACH_OBSERVER(RenderSystemObserver,
                    observers_,
                    OnDestroyed());
}

void RenderSystemObserverManager::CallResizing(RenderSystem* rs) {
  /*
  FOR_EACH_OBSERVER(RenderSystemObserver,
                    observers_,
                    OnResizing(rs));
  */
}

void RenderSystemObserverManager::CallResized(RenderSystem* rs) {
  FOR_EACH_OBSERVER(RenderSystemObserver,
                    observers_,
                    OnResized(rs));
}

void RenderSystemObserverManager::CallPresent(RenderSystem* rs) {
  /*
  FOR_EACH_OBSERVER(RenderSystemObserver,
                    observers_,
                    OnPresent(rs));
  */
}
}  // namespace subtle

void AddRenderSystemObservered(RenderSystemObserver* observer) {
}
void RemoveRenderSystemObservered(RenderSystemObserver* observer) {
}

}  // namespace azer
