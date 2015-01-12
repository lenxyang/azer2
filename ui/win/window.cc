#include "azer/ui/win/window.h"

#include "azer/ui/win/client/screen_position_client.h"

namespace azer {
namespace win {

Window::Window() {
}

Window::~Window() {
}

gfx::Rect Window::GetBoundsInRootWindow() const {
  // TODO(beng): There may be a better way to handle this, and the existing code
  //             is likely wrong anyway in a multi-display world, but this will
  //             do for now.
  if (!GetRootWindow())
    return bounds();
  gfx::Point origin = bounds().origin();
  ConvertPointToTarget(parent_, GetRootWindow(), &origin);
  return gfx::Rect(origin, bounds().size());
}

gfx::Rect Window::GetBoundsInScreen() const {
  gfx::Rect bounds(GetBoundsInRootWindow());
  const Window* root = GetRootWindow();
  if (root) {
    ScreenPositionClient* screen_position_client = GetScreenPositionClient(root);
    if (screen_position_client) {
      gfx::Point origin = bounds.origin();
      screen_position_client->ConvertPointToScreen(root, &origin);
      bounds.set_origin(origin);
    }
  }
  return bounds;
}

// static
void Window::ConvertPointToTarget(const Window* source,
                                  const Window* target,
                                  gfx::Point* point) {
  if (!source)
    return;
  if (source->GetRootWindow() != target->GetRootWindow()) {
    ScreenPositionClient* source_client = 
        GetScreenPositionClient(source->GetRootWindow());
    // |source_client| can be NULL in tests.
    if (source_client)
      source_client->ConvertPointToScreen(source, point);

    ScreenPositionClient* target_client =
        GetScreenPositionClient(target->GetRootWindow());
    // |target_client| can be NULL in tests.
    if (target_client)
      target_client->ConvertPointFromScreen(target, point);
  } else if ((source != target) && (!source->layer() || !target->layer())) {
    if (!source->layer()) {
      gfx::Vector2d offset_to_layer;
      source = source->GetAncestorWithLayer(&offset_to_layer);
      *point += offset_to_layer;
    }
    if (!target->layer()) {
      gfx::Vector2d offset_to_layer;
      target = target->GetAncestorWithLayer(&offset_to_layer);
      *point -= offset_to_layer;
    }
    ui::Layer::ConvertPointToLayer(source->layer(), target->layer(), point);
  } else {
    ui::Layer::ConvertPointToLayer(source->layer(), target->layer(), point);
  }
}

// static
void Window::ConvertRectToTarget(const Window* source,
                                 const Window* target,
                                 gfx::Rect* rect) {
  DCHECK(rect);
  gfx::Point origin = rect->origin();
  ConvertPointToTarget(source, target, &origin);
  rect->set_origin(origin);
}

}  // namespace win
}  // namespace azer
