#include "azer/ui/views/tests/views_test_base.h"

#include "base/logging.h"

#include "ui/gfx/screen.h"
#include "azer/ui/aura/env.h"
#include "azer/ui/views/aura/aura_screen.h"

namespace views {
namespace test {
ViewsTestBase::ViewsTestBase() 
    : setup_called_(false)
    , teardown_called_(false) {
}

ViewsTestBase::~ViewsTestBase() {
  CHECK(setup_called_);
  CHECK(teardown_called_);
}

void ViewsTestBase::SetUp() {
  setup_called_ = true;
  aura::Env::CreateInstance(true);
  aura_screen_.reset(AuraScreen::Create(gfx::Size()));
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, aura_screen_.get());
  message_loop_.reset(new ::base::MessageLoopForUI);
}

void ViewsTestBase::TearDown() {
  teardown_called_ = true;
  /*
  message_loop_.reset();
  root_view_.reset();
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, NULL);
  aura::Env::DeleteInstance();
  */
}

void ViewsTestBase::InitRootView(const RootView::InitParams& params) {
  root_view_.reset(new RootView);
  root_view_->Init(params);
}
}  // namespace test
}  // namespace views
