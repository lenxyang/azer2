#pragma once

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/base/render_export.h"

#define ARRAY_SIZE(x) sizeof(x) / sizeof((x)[0])

namespace azer {
void InitApp(int* argc, char** argv[], const char*);
}  // namespace azer

