#pragma once


#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/events/event_handler.h"
#include "ui/events/event_target.h"

#include "azer/base/export.h"

namespace ui {
class PlatformEventSource;
}

namespace azer {
namespace win {
class Window;
class WindowTreeHost;

class AZER_EXPORT WinContext : public ::ui::EventTarget {
 public:
  // Creates the single Env instance (if it hasn't been created yet). If
  // |create_event_source| is true a PlatformEventSource is created.
  // TODO(sky): nuke |create_event_source|. Only necessary while mojo's
  // nativeviewportservice lives in the same process as the viewmanager.
  static void CreateInstance(bool create_event_source);
  static Env* GetInstance();
  static void DeleteInstance();
 private:
  WinContext();
  ~WinContext();

  // See description of CreateInstance() for deatils of |create_event_source|.
  void Init(bool create_event_source);

  // Overridden from ui::EventTarget:
  bool CanAcceptEvent(const ::ui::Event& event) override;
  ::ui::EventTarget* GetParentTarget() override;
  scoped_ptr< ::ui::EventTargetIterator> GetChildIterator() const override;
  ::ui::EventTargeter* GetEventTargeter() override;

  scoped_ptr< ::ui::PlatformEventSource> event_source_;

  friend class Window;
  friend class WindowTreeHost;
  DISALLOW_COPY_AND_ASSIGN(WinContext);
};
}  // namespace win
}  // namespace azer
