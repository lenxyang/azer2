#pragma once

#include "base/basictypes.h"
#include "azer/base/render_export.h"
#include "azer/ui/event/event.h"

namespace azer {
namespace ui {

/**
 * ��Ϣ���ݵĴ���
 * ��Ϣ�� rootwindow ��ȡ�����ղ�ι�ϵ�·���ָ�����մ��ڴ�����Ϣλ��
 */
class AZER_EXPORT EventTarget : public EventHandler {
 public:
  EventTarget();

  /**
   * Ԥ������Ϣ�������Ϣ���������̼����·����򷵻� false
   */
  virtual bool PreProcessEvent(Event* event) = 0;

  virtual bool CanAcceptEvent(const Event& event) = 0;
  virtual EventTarget* GetParentTarget() = 0;

  // Adds a handler to receive events before the target. The handler must be
  // explicitly removed from the target before the handler is destroyed. The
  // EventTarget does not take ownership of the handler.
  void AddPreTargetHandler(EventHandler* handler);
  // Same as AddPreTargetHandler except that the |handler| is added to the front
  // of the list so it is the first one to receive events.
  void PrependPreTargetHandler(EventHandler* handler);
  void RemovePreTargetHandler(EventHandler* handler);
  // Adds a handler to receive events after the target. The handler must be
  // explicitly removed from the target before the handler is destroyed. The
  // EventTarget does not take ownership of the handler.
  void AddPostTargetHandler(EventHandler* handler);
  void RemovePostTargetHandler(EventHandler* handler);
 private:
  DISALLOW_COPY_AND_ASSIGN(EventTarget);
};
}  // namespace ui
}  // namespace azer
