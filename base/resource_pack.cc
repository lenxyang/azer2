#include "azer/base/resource_pack.h"

#include "base/memory/ref_counted_memory.h"
#include "base/files/file_path.h"
#include "azer/base/string.h"

namespace azer {
ResourcePack::ResourcePack() {
  image_lock_.reset(new ::base::Lock());
}

ResourcePack::~ResourcePack() {}

gfx::Image ResourcePack::GetImageNamed(int resource_id) {
  {
    ::base::AutoLock lock_scope(*image_lock_);
    if (images_.count(resource_id)) {
      return images_[resource_id];
    }
  }

  ::base::RefCountedStaticMemory* memory = datapack_->GetStaticMemory(resource_id);
  if (memory) {
    using gfx::Image;
    Image img = Image::CreateFrom1xPNGBytes(memory->front(), memory->size());
    ::base::AutoLock lock_scope(*image_lock_);
    images_[resource_id] = img;
    return images_[resource_id];
  } else {
    return gfx::Image();
  }
}

const gfx::ImageSkia* ResourcePack::GetImageSkiaNamed(int resource_id) {
  gfx::Image img = GetImageNamed(resource_id);
  return img.ToImageSkia();
}

::base::RefCountedStaticMemory* ResourcePack::LoadDataResourceBytes(
    int resource_id) {
  return datapack_->GetStaticMemory(resource_id);
}

bool ResourcePack::Load(const ::base::FilePath& path) {
  datapack_.reset(new ui::DataPack(ui::ScaleFactor(1)));
  return datapack_->LoadFromPath(path);
}
}  // namespace azer
