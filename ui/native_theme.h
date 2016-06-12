#pragma once

#include "ui/gfx/canvas.h"
#include "ui/native_theme/native_theme.h"
#include "ui/native_theme/native_theme_aura.h"

namespace azer {
class WindowContext;
class NativeTheme : public ui::NativeThemeAura {
 public:
  NativeTheme(WindowContext* context) : context_(context) {}
  WindowContext* context() const { return context_;}
 private:
  WindowContext* context_;
  DISALLOW_COPY_AND_ASSIGN(NativeTheme);
};

class FallbackNativeTheme : public NativeTheme {
 public:
  FallbackNativeTheme(WindowContext* context);
  ~FallbackNativeTheme() override;

  SkColor GetSystemColor(ColorId color_id) const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(FallbackNativeTheme);
};
}  // namespace azer
