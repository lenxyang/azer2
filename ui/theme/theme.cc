#include "azer/ui/theme/theme.h"

#include "base/logging.h"

namespace azer {

Theme* Theme::instance_ = NULL;

Theme::Theme() {
}

Theme::~Theme() {
}

Theme* Theme::GetInstance() {
  DCHECK(instance_);
  return instance_;
}

void Theme::SetInstance(Theme* theme) {
  instance_ = theme;
}

}  // namespace azer
