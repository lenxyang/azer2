#pragma once

#include "ui/views/focus/focus_manager_delegate.h"
#include "ui/views/focus/focus_manager_factory.h"

namespace azer {
class FocusManagerDelegateCreator {
 public:
  virtual views::FocusManagerDelegate* Create() = 0;
};

class FocusManagerFactory : public views::FocusManagerFactory {
 public:
  FocusManagerFactory();

  static FocusManagerFactory* instance();
  static void SetInstance(FocusManagerFactory* instance);

  void SetFocusManagerDelegateCreator(FocusManagerDelegateCreator* creator);
  views::FocusManager* CreateFocusManager(views::Widget* widget,
                                          bool desktop_widget) override;
 private:
  static FocusManagerFactory* instance_;
  FocusManagerDelegateCreator* creator_;
  DISALLOW_COPY_AND_ASSIGN(FocusManagerFactory);
};
}  // namespace azer
