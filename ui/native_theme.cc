#include "azer/ui/native_theme.h"

namespace azer {
FallbackNativeTheme::FallbackNativeTheme(WindowContext* context)
    : NativeTheme(context) {
}

FallbackNativeTheme::~FallbackNativeTheme() {
}

SkColor FallbackNativeTheme::GetSystemColor(ColorId color_id) const {
  switch (color_id) {
    case kColorId_LabelEnabledColor:
      return SkColorSetRGB(245, 245, 245);
    case kColorId_LabelDisabledColor:
      return SkColorSetRGB(149, 149, 149);
    case kColorId_LabelBackgroundColor:
      return SkColorSetRGB(40, 40, 40);
    default:
      return NativeThemeAura::GetSystemColor(color_id);
  }
}
}  // namespace azer
