#pramga once

#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

namespace azer {
namespace resources {
class AZER_EXPORT FileContent : public ::base::RefCounted<FileContent> {
 public:
  FileContent();
  FileContent(const uint8* data, int32 size);

  int64 length() const { return length_;}
  uint8* data() { return data_.get();}
  const uint8* data() const { return data_.get();}
 private:
  int64 length_;
  std::unique_ptr<uint8*> data_;
  DISALLOW_COPY_AND_ASSIGN(FileContent);
};

typedef scoped_ptr<FileContent> FileContentPtr;
}  // namespace resources
}  // namespace azer
