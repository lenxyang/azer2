#pragma once

#include "base/basictypes.h"
#include "ui/native_theme/native_theme.h"
#include "azer/ui/theme/theme_export.h"

namespace azer {
class AZER_THEME_EXPORT Theme : public ui::NativeTheme {
 public:
  ~Theme() override;
  static Theme* GetInstance();
  static void SetInstance(Theme* theme);
 protected:
  Theme();
 private:
  static Theme* instance_;
  DISALLOW_COPY_AND_ASSIGN(Theme);
};
}  // namespace azer
