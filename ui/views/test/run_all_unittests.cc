// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/bind.h"
#include "base/test/launcher/unit_test_launcher.h"
#include "base/test/test_suite.h"

#include "ui/gfx/win/dpi.h"

#include "azer/render/util/render_system_loader.h"

int main(int argc, char** argv) {
  base::TestSuite test_suite(argc, argv);
#if defined(OS_WIN)
  gfx::InitDeviceScaleFactor(1.0f);
#endif
  CHECK(azer::LoadRenderSystem());
  return base::LaunchUnitTests(
      argc,
      argv,
      base::Bind(&base::TestSuite::Run, base::Unretained(&test_suite)));
}