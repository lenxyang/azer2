#pragma once

#include "ui/gfx/native_widget_types.h"
#include "azer/render/swap_chain.h"

namespace aura {
class Window;
class WindowTreeHost;
}  // namespace aura

namespace views {
class Widget;
}  // namespace views

namespace azer {
class WidgetRendererContext;
class TextureOutputDevice;

AZER_EXPORT bool IsWidgetRendererWindow(views::Widget* widget);
void SetWidgetRendererWindow(views::Widget* widget);

AZER_EXPORT TextureOutputDevice* GetTextureOutputDeviceForWidget(views::Widget* widget);
void SetTextureOutputDeviceForWidget(views::Widget* widget,
                                     TextureOutputDevice* device);

AZER_EXPORT WidgetRendererContext* GetWidgetRendererContext(views::Widget* widget);
AZER_EXPORT void SetWidgetRendererContext(views::Widget* widget, WidgetRendererContext* context);


views::Widget* GetWidgetForWindowTreeHost(gfx::AcceleratedWidget widget);
views::Widget* GetWidgetForWindowTreeHost(aura::WindowTreeHost* host);
}  // namespace azer
