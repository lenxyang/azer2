#include "azer/ui/win/client/constants.h"

#include "azer/ui/win/window_property.h"

DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(AURA_EXPORT, bool)
DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(AURA_EXPORT, ui::ModalType)
DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(AURA_EXPORT, gfx::Rect*)
DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(AURA_EXPORT, ui::InputMethod*)
DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(AURA_EXPORT, ui::WindowShowState)

namespace azer {
namespace win {

// Alphabetical sort.

DEFINE_WINDOW_PROPERTY_KEY(bool, kAlwaysOnTopKey, false);
DEFINE_WINDOW_PROPERTY_KEY(bool, kAnimationsDisabledKey, false);
DEFINE_WINDOW_PROPERTY_KEY(bool, kCanMaximizeKey, false);
DEFINE_WINDOW_PROPERTY_KEY(bool, kCanMinimizeKey, false);
DEFINE_WINDOW_PROPERTY_KEY(bool, kCanResizeKey, true);
DEFINE_WINDOW_PROPERTY_KEY(bool, kConstrainedWindowKey, false);
DEFINE_WINDOW_PROPERTY_KEY(bool, kDrawAttentionKey, false);
DEFINE_WINDOW_PROPERTY_KEY(Window*, kHostWindowKey, NULL);
DEFINE_WINDOW_PROPERTY_KEY(ui::ModalType, kModalKey, ui::MODAL_TYPE_NONE);
// gfx::Rect object for RestoreBoundsKey property is owned by the window
// and will be freed automatically.
DEFINE_OWNED_WINDOW_PROPERTY_KEY(gfx::Rect, kRestoreBoundsKey, NULL);
DEFINE_WINDOW_PROPERTY_KEY(
    ui::WindowShowState, kRestoreShowStateKey, ui::SHOW_STATE_DEFAULT);
DEFINE_WINDOW_PROPERTY_KEY(ui::InputMethod*, kRootWindowInputMethodKey, NULL);
DEFINE_WINDOW_PROPERTY_KEY(
    ui::WindowShowState, kShowStateKey, ui::SHOW_STATE_DEFAULT);

}  // namespace win
}  // namespace azer
