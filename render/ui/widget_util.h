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


class TextureOutputDevice;

bool IsWidgetRendererWindow(views::Widget* widget);
void SetWidgetRendererWindow(views::Widget* widget);

TextureOutputDevice* GetTextureOutputDeviceForWidget(views::Widget* widget);
void SetTextureOutputDeviceForWidget(views::Widget* widget,
                                     TextureOutputDevice* device);


views::Widget* GetWidgetForWindowTreeHost(gfx::AcceleratedWidget widget);
views::Widget* GetWidgetForWindowTreeHost(aura::WindowTreeHost* host);
