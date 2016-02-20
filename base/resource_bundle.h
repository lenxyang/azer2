#pragma once

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/synchronization/lock.h"
#include "ui/base/resource/data_pack.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image.h"
#include "ui/base/resource/resource_bundle.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT ResourceBundle : public ui::ResourceBundle::Delegate {
 public:
  ResourceBundle();
  ~ResourceBundle();

  bool Load(const ::base::FilePath& path);

  const gfx::ImageSkia* GetImageSkiaNamed(int resource_id);

  base::FilePath GetPathForResourcePack(const base::FilePath& pack_path,
                                        ui::ScaleFactor scale_factor) override;

  base::FilePath GetPathForLocalePack(const base::FilePath& pack_path,
                                      const std::string& locale) override;

  gfx::Image GetImageNamed(int resource_id) override;
  gfx::Image GetNativeImageNamed(int resource_id,
                                 ui::ResourceBundle::ImageRTL rtl) override;

  base::RefCountedStaticMemory* LoadDataResourceBytes(
      int resource_id, ui::ScaleFactor scale_factor) override;

  bool GetRawDataResource(int resource_id, ui::ScaleFactor scale_factor,
                          base::StringPiece* value) override;
  bool GetLocalizedString(int message_id, base::string16* value) override;
  scoped_ptr<gfx::Font> GetFont(ui::ResourceBundle::FontStyle style) override;
 private:
  std::unique_ptr<ui::DataPack> datapack_;
  typedef std::map<int, gfx::Image> ImageMap;
  ImageMap images_;
  gfx::Image empty_image_;
  scoped_ptr< ::base::Lock> image_lock_;
  DISALLOW_COPY_AND_ASSIGN(ResourceBundle);
};
}  // namespace azer
