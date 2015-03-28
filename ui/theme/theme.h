#pragma once

#include "base/basictypes.h"
#include "ui/native_theme/native_theme.h"
#include "ui/native_theme/fallback_theme.h"

namespace azer {
class AZER_EXPORT Theme : public ui::FallbackTheme {
 public:
  ~Theme() override;
  static Theme* GetInstance();
  static void SetInstance(Theme* theme);

  enum ExtColorId {
    kExtColorId_PanelBackgroundColor,
    kExtColorId_PanelCaptionbarBackground,
    kExtColorId_PanelCaptionbarFocusBackground,
    kExtColorId_CaptionbarText,
    kExtColorId_SubpanelSeperator1,
    kExtColorId_SubpanelSeperator2,
    kExtColorId_MenubarBackground,
    kExtColorId_ButtonGrounpBorder,
    kExtColorId_kDockPanelBackground,
  };
 protected:
  Theme();
 private:
  static Theme* instance_;
  DISALLOW_COPY_AND_ASSIGN(Theme);
};
}  // namespace azer
