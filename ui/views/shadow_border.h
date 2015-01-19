#pragma once

#include "third_party/skia/include/core/SkColor.h"
#include "azer/ui/views/border.h"
#include "azer/base/export.h"

namespace azer {
namespace views {

class View;

// Creates a css box-shadow like border which fades into SK_ColorTRANSPARENT.
class AZER_EXPORT ShadowBorder : public Border {
 public:
  ShadowBorder(int blur,
               SkColor color,
               int vertical_offset,
               int horizontal_offset);
  ~ShadowBorder() override;

 protected:
  // Overridden from views::Border:
  void Paint(const View& view, gfx::Canvas* canvas) override;
  gfx::Insets GetInsets() const override;
  gfx::Size GetMinimumSize() const override;

 private:
  // Blur amount of the shadow in pixels. For details on how blur is defined see
  // comments for blur_ in class ShadowValue.
  const int blur_;

  // Shadow color.
  const SkColor color_;

  // Number of pixels to shift shadow to bottom.
  const int vertical_offset_;

  // Number of pixels to shift shadow to right.
  const int horizontal_offset_;

  DISALLOW_COPY_AND_ASSIGN(ShadowBorder);
};

}  // namespace views
}  // namespace azer

