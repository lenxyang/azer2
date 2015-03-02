// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/views/test/menu_runner_test_api.h"

#include "azer/ui/views/controls/menu/menu_runner.h"
#include "azer/ui/views/controls/menu/menu_runner_handler.h"

namespace views {
namespace test {

MenuRunnerTestAPI::MenuRunnerTestAPI(MenuRunner* menu_runner)
    : menu_runner_(menu_runner) {
}

MenuRunnerTestAPI::~MenuRunnerTestAPI() {
}

void MenuRunnerTestAPI::SetMenuRunnerHandler(
    scoped_ptr<MenuRunnerHandler> menu_runner_handler) {
  menu_runner_->SetRunnerHandler(menu_runner_handler.Pass());
}

}  // test
}  // views
