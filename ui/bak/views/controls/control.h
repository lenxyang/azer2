#pragma once

#include "base/strings/string16.h"
#include "ui/gfx/geometry/point.h"

#include "azer/ui/views/view.h"

namespace azer {
namespace views {
class AZER_EXPORT Control : public View {
 public:
  virtual bool GetTooltipText(const gfx::Point& p, base::string16* tooltip) const {
    return false;
  }

  void SetTooltipText(const base::string16& tooltip_text);
 protected:
  Control(View* parent);
  virtual ~Control();

  // The text shown in a tooltip.
  base::string16 tooltip_text_;
  DISALLOW_COPY_AND_ASSIGN(Control);
};
}  // namespace views
}  // namespace azer
