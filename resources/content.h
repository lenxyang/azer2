#pragma once

#include <utility>
#include <memory>
#include "azer/base/export.h"

namespace azer {

struct AZER_EXPORT FileContent {
  int64 length;
  char data[1];
};

typedef std::shared_ptr<FileContent> FileContentPtr;
}  // namespace azer
