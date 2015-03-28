#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/render_text.h"
#include "ui/compositor/debug_utils.h"
#include "ui/base/models/combobox_model.h"
#include "ui/base/models/simple_menu_model.h"
#include "azer/ui/views/background.h" 
#include "azer/ui/views/controls/label.h"
#include "azer/ui/views/controls/button/blue_button.h"
#include "azer/ui/views/controls/button/checkbox.h"
#include "azer/ui/views/controls/button/label_button.h"
#include "azer/ui/views/controls/button/radio_button.h"
#include "azer/ui/views/controls/button/menu_button.h"
#include "azer/ui/views/controls/button/menu_button_listener.h"
#include "azer/ui/views/controls/combobox/combobox.h"
#include "azer/ui/views/controls/combobox/combobox_listener.h"
#include "azer/ui/views/controls/menu/menu_runner.h"
#include "azer/ui/views/controls/textfield/textfield.h"
#include "azer/ui/views/controls/textfield/textfield_controller.h"
#include "azer/ui/views/layout/grid_layout.h"
#include "azer/ui/views/widget/widget.h"
#include "azer/ui/views/widget/widget_delegate.h"

#include "azer/ui/render_loop.h"
#include "azer/ui/uienv.h"