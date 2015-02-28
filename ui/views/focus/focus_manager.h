#pragma once

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "azer/ui/views/views_export.h"

namespace views {

class Focusable;

class VIEWS_EXPORT FocusManager {
 public:
  FocusManager();
  ~FocusManager();
  Focusable* GetFocused() { return focused_;}
  const Focusable* GetFocused() const { return focused_;}

  void SetFocused(Focusable* focusable);
  void ClearFocus();

  // Register a keyboard accelerator for the specified target. If multiple
  // targets are registered for an accelerator, a target registered later has
  // higher priority.
  // |accelerator| is the accelerator to register.
  // |priority| denotes the priority of the handler.
  // NOTE: In almost all cases, you should specify kNormalPriority for this
  // parameter. Setting it to kHighPriority prevents Chrome from sending the
  // shortcut to the webpage if the renderer has focus, which is not desirable
  // except for very isolated cases.
  // |target| is the AcceleratorTarget that handles the event once the
  // accelerator is pressed.
  // Note that we are currently limited to accelerators that are either:
  // - a key combination including Ctrl or Alt
  // - the escape key
  // - the enter key
  // - any F key (F1, F2, F3 ...)
  // - any browser specific keys (as available on special keyboards)
  void RegisterAccelerator(const ui::Accelerator& accelerator,
                           ui::AcceleratorManager::HandlerPriority priority,
                           ui::AcceleratorTarget* target);

  // Unregister the specified keyboard accelerator for the specified target.
  void UnregisterAccelerator(const ui::Accelerator& accelerator,
                             ui::AcceleratorTarget* target);

  // Unregister all keyboard accelerator for the specified target.
  void UnregisterAccelerators(ui::AcceleratorTarget* target);

  // Activate the target associated with the specified accelerator.
  // First, AcceleratorPressed handler of the most recently registered target
  // is called, and if that handler processes the event (i.e. returns true),
  // this method immediately returns. If not, we do the same thing on the next
  // target, and so on.
  // Returns true if an accelerator was activated.
  bool ProcessAccelerator(const ui::Accelerator& accelerator);

  // Returns the next focusable view. Traversal starts at |starting_view|. If
  // |starting_view| is NULL |starting_widget| is consuled to determine which
  // Widget to start from. See
  // WidgetDelegate::ShouldAdvanceFocusToTopLevelWidget() for details. If both
  // |starting_view| and |starting_widget| are NULL, traversal starts at
  // |widget_|.
  Focusable* GetNextFocusableView(View* starting_view, 
                                  bool reverse, bool dont_loop);
 private:
  Focusable* focused_;
  std::map<int64, FocusGroup*> groups_;
  DISALLOW_COPY_AND_ASSIGN(FocusManager);
};
}  // namespace views
