#include "azer/ui/views/aura/aura_screen.h"

#include "base/logging.h"
#include "azer/ui/aura/env.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_event_dispatcher.h"
#include "azer/ui/aura/window_tree_host.h"
#include "ui/gfx/geometry/size_conversions.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect_conversions.h"
#include "ui/gfx/screen.h"

namespace views {

namespace {

bool IsRotationPortrait(gfx::Display::Rotation rotation) {
  return rotation == gfx::Display::ROTATE_90 ||
         rotation == gfx::Display::ROTATE_270;
}

}  // namespace

// static
AuraScreen* AuraScreen::Create(const gfx::Size& size) {
  const gfx::Size kDefaultSize(800, 600);
  // Use (0,0) because the desktop aura tests are executed in
  // native environment where the display's origin is (0,0).
  return new AuraScreen(gfx::Rect(size.IsEmpty() ? kDefaultSize : size));
}

// static
AuraScreen* AuraScreen::CreateFullscreen() {
  return new AuraScreen(gfx::Rect(aura::WindowTreeHost::GetNativeScreenSize()));
}

AuraScreen::~AuraScreen() {
}

aura::WindowTreeHost* AuraScreen::CreateHostForPrimaryDisplay() {
  DCHECK(!host_);
  host_ = aura::WindowTreeHost::Create(gfx::Rect(display_.GetSizeInPixel()));
  host_->window()->AddObserver(this);
  host_->InitHost();
  return host_;
}

void AuraScreen::SetDeviceScaleFactor(float device_scale_factor) {
  gfx::Rect bounds_in_pixel(display_.GetSizeInPixel());
  display_.SetScaleAndBounds(device_scale_factor, bounds_in_pixel);
  // host_->OnHostResized(bounds_in_pixel.size());
}

void AuraScreen::SetDisplayRotation(gfx::Display::Rotation rotation) {
  gfx::Rect bounds_in_pixel(display_.GetSizeInPixel());
  gfx::Rect new_bounds(bounds_in_pixel);
  if (IsRotationPortrait(rotation) != IsRotationPortrait(display_.rotation())) {
    new_bounds.set_width(bounds_in_pixel.height());
    new_bounds.set_height(bounds_in_pixel.width());
  }
  display_.set_rotation(rotation);
  display_.SetScaleAndBounds(display_.device_scale_factor(), new_bounds);
  host_->SetRootTransform(GetRotationTransform() * GetUIScaleTransform());
}

void AuraScreen::SetUIScale(float ui_scale) {
  ui_scale_ = ui_scale;
  gfx::Rect bounds_in_pixel(display_.GetSizeInPixel());
  gfx::Rect new_bounds = gfx::ToNearestRect(
      gfx::ScaleRect(bounds_in_pixel, 1.0f / ui_scale));
  display_.SetScaleAndBounds(display_.device_scale_factor(), new_bounds);
  host_->SetRootTransform(GetRotationTransform() * GetUIScaleTransform());
}

void AuraScreen::SetWorkAreaInsets(const gfx::Insets& insets) {
  display_.UpdateWorkAreaFromInsets(insets);
}

gfx::Transform AuraScreen::GetRotationTransform() const {
  gfx::Transform rotate;
  switch (display_.rotation()) {
    case gfx::Display::ROTATE_0:
      break;
    case gfx::Display::ROTATE_90:
      rotate.Translate(display_.bounds().height(), 0);
      rotate.Rotate(90);
      break;
    case gfx::Display::ROTATE_270:
      rotate.Translate(0, display_.bounds().width());
      rotate.Rotate(270);
      break;
    case gfx::Display::ROTATE_180:
      rotate.Translate(display_.bounds().width(),
                       display_.bounds().height());
      rotate.Rotate(180);
      break;
  }

  return rotate;
}

gfx::Transform AuraScreen::GetUIScaleTransform() const {
  gfx::Transform ui_scale;
  ui_scale.Scale(1.0f / ui_scale_, 1.0f / ui_scale_);
  return ui_scale;
}

void AuraScreen::OnWindowBoundsChanged(
    aura::Window* window, const gfx::Rect& old_bounds, const gfx::Rect& new_bounds) {
  DCHECK_EQ(host_->window(), window);
  display_.SetSize(gfx::ToFlooredSize(
      gfx::ScaleSize(new_bounds.size(), display_.device_scale_factor())));
}

void AuraScreen::OnWindowDestroying(aura::Window* window) {
  if (host_->window() == window)
    host_ = NULL;
}

gfx::Point AuraScreen::GetCursorScreenPoint() {
  return aura::Env::GetInstance()->last_mouse_location();
}

gfx::NativeWindow AuraScreen::GetWindowUnderCursor() {
  return GetWindowAtScreenPoint(GetCursorScreenPoint());
}

gfx::NativeWindow AuraScreen::GetWindowAtScreenPoint(const gfx::Point& point) {
  return host_->window()->GetTopWindowContainingPoint(point);
}

int AuraScreen::GetNumDisplays() const {
  return 1;
}

std::vector<gfx::Display> AuraScreen::GetAllDisplays() const {
  return std::vector<gfx::Display>(1, display_);
}

gfx::Display AuraScreen::GetDisplayNearestWindow(
    gfx::NativeWindow window) const {
  return display_;
}

gfx::Display AuraScreen::GetDisplayNearestPoint(const gfx::Point& point) const {
  return display_;
}

gfx::Display AuraScreen::GetDisplayMatching(const gfx::Rect& match_rect) const {
  return display_;
}

gfx::Display AuraScreen::GetPrimaryDisplay() const {
  return display_;
}

void AuraScreen::AddObserver(gfx::DisplayObserver* observer) {
}

void AuraScreen::RemoveObserver(gfx::DisplayObserver* observer) {
}

AuraScreen::AuraScreen(const gfx::Rect& screen_bounds)
    : host_(NULL),
      ui_scale_(1.0f) {
  static int64 synthesized_display_id = 2000;
  display_.set_id(synthesized_display_id++);
  display_.SetScaleAndBounds(1.0f, screen_bounds);
}

}  // namespace views
