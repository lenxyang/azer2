#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "azer/base/export.h"

namespace azer {
class FileContent;

typedef scoped_refptr<FileContent> FileContentPtr;

class AZER_EXPORT FileContentCache {
 public:
 private:
  DISALLOW_COPY_AND_ASSIGN(FileContentCache);
};
}  // namespace azer
