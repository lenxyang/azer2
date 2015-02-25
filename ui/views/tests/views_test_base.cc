#include "azer/ui/views/tests/views_test_base.h"

#include "base/logging.h"

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
}

void ViewsTestBase::TearDown() {
  teardown_called_ = true;
}

void ViewsTestBase::InitRootView(const RootView::InitParams& params) {
  root_view_.reset(new RootView);
  root_view_->Init(params);
}
}  // namespace test
}  // namespace views
