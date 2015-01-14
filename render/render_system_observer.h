#pragma once

#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT RenderSystemObserver {
 public:
  virtual void OnCreated(RenderSystem* render_system) {}
  // 销毁时触发
  virtual void OnDestroying(RenderSystem* render_system) {}
  virtual void OnDestroyed() {}

  // 由窗口触发
  virtual void OnResizing(RenderSystem* render_system) {}
  virtual void OnResized(RenderSystem* render_system) {}

  // 展示时触发
  virtual void OnPresent(RenderSystem* render_system) {}
 protected:
  virtual ~RenderSystemObserver() {}
};

void AZER_EXPORT AddRenderSystemObservered(RenderSystemObserver* observer);
void AZER_EXPORT RemoveRenderSystemObservered(RenderSystemObserver* observer);

namespace subtle {
class AZER_EXPORT RenderSystemObserverManager {
 public:
  void CallCreated(RenderSystem* rs);
  void CallDestroying(RenderSystem* render_system);
  void CallDestroyed(RenderSystem* render_system);
  void CallResizing(RenderSystem* render_system);
  void CallResized(RenderSystem* render_system);
  void CallPresent(RenderSystem* render_system);
 private:
  RenderSystemObserverManager();
  ~RenderSystemObserverManager();

  ObserverList<RenderSystemObserverManager> observers_;
  DISALLOW_COPY_AND_ASSIGN(RenderSystemObserverManager);
};
}  // namespace subtle
}  // namespace azer
