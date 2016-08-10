#include "azer/ui/focus_manager_factory.h"

#include "ui/views/focus/focus_manager.h"

namespace azer {
FocusManagerFactory* FocusManagerFactory::instance_ = NULL;
FocusManagerFactory* FocusManagerFactory::instance() {
  return instance_;
}

void FocusManagerFactory::SetInstance(FocusManagerFactory* instance) {
  if (instance_) 
    delete instance_;
  instance_ = instance;
  views::FocusManagerFactory::Install(instance_);
}

FocusManagerFactory::FocusManagerFactory()
    : creator_(NULL) {
}

void FocusManagerFactory::SetFocusManagerDelegateCreator(
    FocusManagerDelegateCreator* creator) {
  creator_ = creator;
}

views::FocusManager* FocusManagerFactory::CreateFocusManager(
    views::Widget* widget, bool desktop_widget) {
  views::FocusManagerDelegate* delegate = (creator_ ? creator_->Create() : NULL);
  return new views::FocusManager(widget, delegate);
}
}  // namespace azer
