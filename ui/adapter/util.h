#pragma once

#include "ui/gfx/native_widget_types.h"
#include "ui/views/widget/native_widget_private.h"

namespace aura {
class WindowTreeHost;
}

namespace azer {
class SwapchainContext;
class TextureOutputDevice;

bool IsRendererWindow(views::Widget* widget);
void SetRendererWindow(views::Widget* widget);
void SetRendererWindow(views::internal::NativeWidgetPrivate* widget);

SwapchainContext* GetSwapchainContext(views::Widget* widget);
void SetSwapchainContext(views::Widget* widget, SwapchainContext* context);

TextureOutputDevice* GetTextureOutputDeviceForWidget(views::Widget* widget);
void SetTextureOutputDeviceForWidget(views::Widget* widget,
                                     TextureOutputDevice* device);

views::Widget* GetWidgetForWindowTreeHost(gfx::AcceleratedWidget widget);
views::Widget* GetWidgetForWindowTreeHost(aura::WindowTreeHost* host);
}  // namespace azer
