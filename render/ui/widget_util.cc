#include "azer/uisbox/adapter/base/widget_util.h"

#include "ui/aura/window.h"
#include "ui/aura/window_tree_host.h"
#include "ui/views/widget/widget.h"

const char* kAzerRenderWindow = "kAzerRenderWindow";
const char* kAzerTextureOutputDevice = "kAzerTextureOutputDevice";

bool IsWidgetRendererWindow(views::Widget* widget) {
  return NULL != widget->GetNativeWindowProperty(kAzerRenderWindow);
}

void SetWidgetRendererWindow(views::Widget* widget) {
  widget->SetNativeWindowProperty(kAzerRenderWindow, (void*)1);
}

TextureOutputDevice* GetTextureOutputDeviceForWidget(views::Widget* widget) {
  return (TextureOutputDevice*)
      widget->GetNativeWindowProperty(kAzerTextureOutputDevice);
}

void SetTextureOutputDeviceForWidget(views::Widget* widget,
                                     TextureOutputDevice* device) {
  widget->SetNativeWindowProperty(kAzerTextureOutputDevice, device);
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
