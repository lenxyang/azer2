#pragma once

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/synchronization/lock.h"
#include "ui/base/resource/data_pack.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT ResourcePack {
 public:
  ResourcePack();
  ~ResourcePack();

  bool Load(const ::base::FilePath& path);

  gfx::Image ResourcePack::GetImageNamed(int resource_id);
  const gfx::ImageSkia* GetImageSkiaNamed(int resource_id);
  ::base::RefCountedStaticMemory* LoadDataResourceBytes(int resource_id);
private:
  std::unique_ptr<ui::DataPack> datapack_;
  typedef std::map<int, gfx::Image> ImageMap;
  ImageMap images_;
  gfx::Image empty_image_;
  scoped_ptr< ::base::Lock> image_lock_;
  DISALLOW_COPY_AND_ASSIGN(ResourcePack);
};
}  // namespace azer
