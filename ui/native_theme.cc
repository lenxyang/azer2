#include "azer/ui/native_theme.h"

namespace azer {
FallbackNativeTheme::FallbackNativeTheme(WindowContext* context)
    : NativeTheme(context) {
}

FallbackNativeTheme::~FallbackNativeTheme() {
}

SkColor FallbackNativeTheme::GetSystemColor(ColorId color_id) const {
  switch (color_id) {
    case kColorId_LabelBackgroundColor:
      return SkColorSetRGBA(0, 0, 0, 0);
    case kColorId_LabelDisabledColor:
      return SkColorSetRGB(149, 149, 149);
    case kColorId_LabelEnabledColor:
      return SkColorSetRGB(245, 245, 245);
    
    case kColorId_ButtonEnabledColor:
    case kColorId_TextOnCallToActionColor:
    case kColorId_ResultsTableHoveredBackground:
    case kColorId_ResultsTableSelectedBackground:
    case kColorId_ResultsTableNormalUrl:
    case kColorId_ResultsTableHoveredUrl:
    case kColorId_ResultsTableSelectedUrl:
      return SkColorSetRGB(245, 245, 245);
    
    case kColorId_ButtonBackgroundColor:
      return SkColorSetRGBA(0, 0, 0, 0);
    case kColorId_ButtonDisabledColor:
      return SkColorSetRGB(149, 149, 149);
    case kColorId_ButtonHighlightColor:
      return SkColorSetRGB(245, 245, 245);
    case kColorId_ButtonHoverColor:
      return SkColorSetRGB(245, 245, 245);
    case kColorId_ButtonHoverBackgroundColor:
      return SkColorSetRGB(57, 57, 57);
    default:
      return NativeThemeAura::GetSystemColor(color_id);
  }
}
}  // namespace azer
