#pragma once

#include "ui/base/ui_base_types.h"
#include "azer/ui/win/window.h"

namespace ui {
class InputMethod;
}

namespace azer {
namespace win {
// Alphabetical sort.

// A property key to store always-on-top flag.
AZER_EXPORT extern const WindowProperty<bool>* const kAlwaysOnTopKey;

// A property key to store whether animations are disabled for the window. Type
// of value is an int.
AZER_EXPORT extern const WindowProperty<bool>* const kAnimationsDisabledKey;

// A property key to store the can-maximize flag.
AZER_EXPORT extern const WindowProperty<bool>* const kCanMaximizeKey;

// A property key to store the can-minimize flag.
AZER_EXPORT extern const WindowProperty<bool>* const kCanMinimizeKey;

// A property key to store the can-resize flag.
AZER_EXPORT extern const WindowProperty<bool>* const kCanResizeKey;

// A property key to store if a window is a constrained window or not.
AZER_EXPORT extern const WindowProperty<bool>* const kConstrainedWindowKey;

// A property key to indicate that a window should show that it deserves
// attention.
AZER_EXPORT extern const WindowProperty<bool>* const kDrawAttentionKey;

// A property key to store the host window of a window. This lets
// WebContentsViews find the windows that should constrain NPAPI plugins.
AZER_EXPORT extern const WindowProperty<Window*>* const kHostWindowKey;

// A property key to store the window modality.
AZER_EXPORT extern const WindowProperty<ui::ModalType>* const kModalKey;

// A property key to store the restore bounds for a window.
AZER_EXPORT extern const WindowProperty<gfx::Rect*>* const kRestoreBoundsKey;

// A property key to store ui::WindowShowState for restoring a window.
// Used in Ash to remember the show state before the window was minimized.
AZER_EXPORT extern const WindowProperty<ui::WindowShowState>* const
    kRestoreShowStateKey;

// A property key to store an input method object that handles a key event.
AZER_EXPORT extern const WindowProperty<ui::InputMethod*>* const
    kRootWindowInputMethodKey;

// A property key to store ui::WindowShowState for a window.
// See ui/base/ui_base_types.h for its definition.
AZER_EXPORT extern const WindowProperty<ui::WindowShowState>* const
    kShowStateKey;

// Alphabetical sort.
}  // namespace win
}  // namespace azer
