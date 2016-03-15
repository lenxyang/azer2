#include "azer/effect/resource_bundle.h"

#include "base/memory/ref_counted_memory.h"
#include "base/files/file_path.h"
#include "azer/base/string.h"

namespace azer {
namespace base {
ResourceBundle::ResourceBundle() {
  image_lock_.reset(new ::base::Lock());
}

ResourceBundle::~ResourceBundle() {}

base::FilePath ResourceBundle::GetPathForResourcePack(
    const base::FilePath& pack_path,
    ui::ScaleFactor scale_factor) {
  return pack_path;
}

base::FilePath ResourceBundle::GetPathForLocalePack(const base::FilePath& pack_path,
                                                    const std::string& locale) {
  return pack_path;
}

gfx::Image ResourceBundle::GetImageNamed(int resource_id) {
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

const gfx::ImageSkia* ResourceBundle::GetImageSkiaNamed(int resource_id) {
  gfx::Image img = GetImageNamed(resource_id);
  return img.ToImageSkia();
}

gfx::Image ResourceBundle::GetNativeImageNamed(int resource_id,
                                               ui::ResourceBundle::ImageRTL rtl) {
  return gfx::Image();
}

base::RefCountedStaticMemory* ResourceBundle::LoadDataResourceBytes(
    int resource_id,
    ui::ScaleFactor scale_factor) {
  return datapack_->GetStaticMemory(resource_id);
}

bool ResourceBundle::GetRawDataResource(int resource_id,
                                        ui::ScaleFactor scale_factor,
                                        base::StringPiece* value) {
  return false;
}

bool ResourceBundle::GetLocalizedString(int message_id, base::string16* value) {
  return false;
}

scoped_ptr<gfx::Font> ResourceBundle::GetFont(ui::ResourceBundle::FontStyle style) {
  return NULL;
}

bool ResourceBundle::Load(const ::base::FilePath& path) {
  datapack_.reset(new ui::DataPack(ui::ScaleFactor(1)));
  return datapack_->LoadFromPath(path);
}
}  // namespace base
}  // namespace azer
