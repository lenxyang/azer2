#pragma once

#include "base/basictypes.h"
#include "base/i18n/rtl.h"
#include "base/strings/string16.h"
#include "ui/accessibility/ax_view_state.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/text_elider.h"
#include "ui/gfx/text_utils.h"
#include "ui/gfx/utf16_indexing.h"
#include "ui/native_theme/native_theme.h"

#include "azer/ui/views/view.h"

namespace views {
class VIEWS_EXPORT Control : public View {
 public:
  Control();
  ~Control() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(Control);
};
}  // namespace views
