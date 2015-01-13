#include "azer/ui/win/context.h"

#include "base/lazy_instance.h"
#include "base/logging.h"
#include "base/threading/thread_local.h"
#include "ui/events/event_target_iterator.h"
#include "ui/events/platform/platform_event_source.h"

namespace azer {
namespace win {

base::LazyInstance<base::ThreadLocalPointer<WinContext> >::Leaky lazy_tls_ptr =
    LAZY_INSTANCE_INITIALIZER;

void WinContext::CreateInstance(bool create_event_source) {
  if (!lazy_tls_ptr.Pointer()->Get())
    (new WinContext())->Init(create_event_source);
}

WinContext* WinContext::GetInstance() {
  WinContext* context = lazy_tls_ptr.Pointer()->Get();
  DCHECK(context) << "Env::CreateInstance must be called before getting the "
      "instance of Env.";
  return context; 
}

// static
void WinContext::DeleteInstance() {
  delete lazy_tls_ptr.Pointer()->Get();
}

WinContext::WinContext() {
  DCHECK(lazy_tls_ptr.Pointer()->Get() == NULL);
  lazy_tls_ptr.Pointer()->Set(this);
}

WinContext::~WinContext() {
  DCHECK_EQ(this, lazy_tls_ptr.Pointer()->Get());
  lazy_tls_ptr.Pointer()->Set(NULL);
}

void WinContext::Init(bool create_event_source) {
#if defined(USE_OZONE)
  // The ozone platform can provide its own event source. So initialize the
  // platform before creating the default event source.
  ui::OzonePlatform::InitializeForUI();
#endif
  if (create_event_source && !ui::PlatformEventSource::GetInstance())
    event_source_ = ui::PlatformEventSource::CreateDefault();
}


bool WinContext::CanAcceptEvent(const ui::Event& event) {
  return true;
}

::ui::EventTarget* WinContext::GetParentTarget() {
  return NULL;
}

scoped_ptr< ::ui::EventTargetIterator> WinContext::GetChildIterator() const {
  return scoped_ptr<ui::EventTargetIterator>();
}

::ui::EventTargeter* WinContext::GetEventTargeter() {
  NOTREACHED();
  return NULL;
}


}  // namespace win
}  // namespace azer
