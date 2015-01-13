#pragma once

#include "azer/base/export.h"

namespace ui {
class Event;
}

namespace azer {
namespace win {

class Window;
class AZER_EXPORT FocusChangeObserver {
 public:
  // Called when focus moves from |lost_focus| to |gained_focus|.
  virtual void OnWindowFocused(Window* gained_focus, Window* lost_focus) = 0;

 protected:
  virtual ~FocusChangeObserver() {}
};

AZER_EXPORT FocusChangeObserver* GetFocusChangeObserver(Window* window);
AZER_EXPORT void SetFocusChangeObserver(Window* window,
                                        FocusChangeObserver* focus_change_observer);

// An interface implemented by an object that manages window focus.
class AZER_EXPORT FocusClient {
 public:
  virtual ~FocusClient() {}

  virtual void AddObserver(FocusChangeObserver* observer) = 0;
  virtual void RemoveObserver(FocusChangeObserver* observer) = 0;

  // Focuses |window|. Passing NULL clears focus.
  virtual void FocusWindow(Window* window) = 0;

  // Sets focus to |window| if it's within the active window. Not intended as a
  // general purpose API, use FocusWindow() instead.
  virtual void ResetFocusWithinActiveWindow(Window* window) = 0;

  // Retrieves the focused window, or NULL if there is none.
  virtual Window* GetFocusedWindow() = 0;
};

// Sets/Gets the focus client on the root Window.
AZER_EXPORT void SetFocusClient(Window* root_window, FocusClient* client);
AZER_EXPORT FocusClient* GetFocusClient(Window* window);
AZER_EXPORT FocusClient* GetFocusClient(const Window* window);


}  // namespace win
}  // namespace azer
