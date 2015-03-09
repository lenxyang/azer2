#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/base/export.h"
#include "azer/base/string.h"
#include "azer/resources/content.h"
#include "azer/resources/file_path.h"
#include "base/lazy_instance.h"

namespace azer {
class FileSystem;

struct AZER_EXPORT FileContent {
  int64 length;
  char* data;
};

class AZER_EXPORT FileSystem {
 public:
  virtual ~FileSystem() {}

  enum Type {
    kNativeFS = 1,
    kPackaged,
    kCompressedPackaged,
  };

  static void InitDefaultFileSystem(const ::base::FilePath& root,
                                    Type type = kNativeFS);
  static void InitDefaultFileSystem(const StringType& root,Type type = kNativeFS);
  static FileSystem* GetDefaultFileSystem();

  static FileSystem* create(Type type, const ::base::FilePath& root);
  const ::base::FilePath& root() { return fs_root_;}

  virtual FileContent* LoadFile(const ResFilePath& path) = 0;
  virtual bool IsPathExists(const ResFilePath& path) = 0;

  // Load content async
  // virtual ResLoadFileAsync(const ResFilePath& path, FileContent* filecontent) = 0;
 protected:
  FileSystem(Type type, const ::base::FilePath& root)
      : type_(type)
      , fs_root_(root) {}

  const Type type_;
  const ::base::FilePath fs_root_;

  friend struct ::base::DefaultLazyInstanceTraits<FileSystem>;
  DISALLOW_COPY_AND_ASSIGN(FileSystem);
};

typedef std::shared_ptr<FileSystem> FileSystemPtr;
}  // namespace azer
