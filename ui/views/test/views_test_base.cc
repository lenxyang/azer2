#include "azer/ui/views/test/views_test_base.h"

#include "base/logging.h"
#include "ui/base/ime/input_method_initializer.h"

#include "azer/ui/aura/env.h"
#include "azer/ui/views/util/aura_screen.h"

namespace views {
ViewsTestBase::ViewsTestBase() 
    : setup_called_(false)
    , teardown_called_(false) {
}

ViewsTestBase::~ViewsTestBase() {
  CHECK(setup_called_);
  CHECK(teardown_called_);
}

void ViewsTestBase::SetUp() {
  aura::Env::CreateInstance(true);
  aura_screen_.reset(AuraScreen::Create(gfx::Size()));
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, aura_screen_.get());
  ui::InitializeInputMethodForTesting();
  setup_called_ = true;
}

void ViewsTestBase::TearDown() {
  aura::Env::DeleteInstance();
  ui::ShutdownInputMethodForTesting();
  teardown_called_ = true;
}

}  // namespace views
