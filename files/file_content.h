#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT FileContent : public ::base::RefCounted<FileContent> {
 public:
  FileContent();
  explicit FileContent(int64 capability);
  FileContent(const uint8* data, int64 size);
  FileContent(scoped_ptr<uint8[]> data, int64 size);

  ~FileContent();

  int64 length() const { return length_;}
  int64 capability() const { return capability_;}
  uint8* data() { return data_.get();}
  const uint8* data() const { return data_.get();}
 private:
  int64 length_;
  int64 capability_;
  scoped_ptr<uint8[]> data_;
  DISALLOW_COPY_AND_ASSIGN(FileContent);
};

typedef scoped_refptr<FileContent> FileContentPtr;
}  // namespace azer
