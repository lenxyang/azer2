#include "azer/ui/adapter/util.h"

#include "ui/aura/window.h"
#include "ui/aura/window_tree_host.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/native_widget_private.h"

namespace azer {
namespace {
const char* kRenderWindow = "azer::ui::kAzerRenderWindow";
const char* kTextureOutputDevice = "azer::ui::kAzerTextureOutputDevice";
const char* kSwapchainContext = "azer::ui:::AzerSwapchainContext";
}  // namespace

bool IsRendererWindow(views::Widget* widget) {
  return NULL != widget->GetNativeWindowProperty(kRenderWindow);
}

void SetRendererWindow(views::Widget* widget) {
  widget->SetNativeWindowProperty(kRenderWindow, (void*)1);
}

void SetRendererWindow(views::internal::NativeWidgetPrivate* widget) {
  widget->SetNativeWindowProperty(kRenderWindow, (void*)1);
}

SwapchainContext* GetSwapchainContext(views::Widget* widget) {
  return (SwapchainContext*)
      widget->GetNativeWindowProperty(kSwapchainContext);
}

void SetSwapchainContext(views::Widget* widget, SwapchainContext* context) {
  widget->SetNativeWindowProperty(kSwapchainContext, context);
}

TextureOutputDevice* GetTextureOutputDeviceForWidget(views::Widget* widget) {
  return (TextureOutputDevice*)
      widget->GetNativeWindowProperty(kTextureOutputDevice);
}

void SetTextureOutputDeviceForWidget(views::Widget* widget,
                                     TextureOutputDevice* device) {
  widget->SetNativeWindowProperty(kTextureOutputDevice, device);
}

views::Widget* GetWidgetForWindowTreeHost(gfx::AcceleratedWidget widget) {
  aura::WindowTreeHost* window_tree_host =
      aura::WindowTreeHost::GetForAcceleratedWidget(widget);
  DCHECK(window_tree_host);
  DCHECK(window_tree_host->window());
  return GetWidgetForWindowTreeHost(window_tree_host);
}

views::Widget* GetWidgetForWindowTreeHost(aura::WindowTreeHost* host) {
  aura::Window* window = host->window();
  while (true) {
    views::Widget* widget = views::Widget::GetWidgetForNativeView(window);
    if (widget) { 
      return widget;
    }

    if (window->children().size() > 0u) {
      window = window->children()[0];
    } else {
      break;
    }
  }
  return NULL;
}
}  // namespace azer
