#pragma once

namespace azer {
namespace widget {
class AZER_EXPORT WindowObserver {
 public:
 protected:
  WindowObserver() {}
  virtual ~WindowObserver() {}
  DISALLOW_COPY_AND_ASSIGN(WindowObserver);
};
}  // namespace widget
}  // namespace azer
