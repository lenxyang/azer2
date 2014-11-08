#pragma once

#include "azer/base/export.h"
#include "azer/ui/window/native_handle.h"


namespace azer {

class WindowHost;

bool AZER_EXPORT LoadRenderSystem(WindowHost* host);
}  // namespace azer
