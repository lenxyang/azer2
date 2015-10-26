#include "azer/base/file_util.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"

namespace azer {
bool ReadFileToString(const ::base::FilePath& path, std::string* content,
                      int max_size) {
  CHECK(content != NULL);
  content->clear();
  FILE* file = ::base::OpenFile(path, "rb");
  if (!file) {
    return false;
  }

  const size_t kBufferSize = 1 << 16;
  scoped_ptr<char[]> buf(new char[kBufferSize]);
  size_t len;
  size_t size = 0;
  bool read_status = true;

  // Many files supplied in |path| have incorrect size (proc files etc).
  // Hence, the file is read sequentially as opposed to a one-shot read.
  while ((len = fread(buf.get(), 1, kBufferSize, file)) > 0) {
    if (content)
      content->append(buf.get(), std::min(len, max_size - size));

    if ((max_size - size) < len) {
      read_status = false;
      break;
    }

    size += len;
  }
  read_status = read_status && !ferror(file);
  ::base::CloseFile(file);

  return read_status;
}
}  // namespace azer 
