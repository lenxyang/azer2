#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "ui/accessibility/ax_enums.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/drop_target_event.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/ui_base_types.h"
#include "ui/events/event.h"
#include "ui/events/event_target.h"
#include "ui/gfx/geometry/r_tree.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d.h"

#include "azer/base/export.h"
#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/views/cull_set.h"

namespace gfx {
class Canvas;
class Insets;
class Path;
class Transform;
}


namespace azer {

namespace compositor {
class Layer;
}

namespace widget {
class Widget;
}  // namespace widget

namespace views {

class Border;
class Background;

class AZER_EXPORT View : public widget::WidgetDelegate {
 public:
  typedef std::vector<View> Views;
  View();
  ~View() override;
  
  virtual void Show();
  virtual void Hide();
  bool visible() const { return visible_;}

  // Size and disposition ------------------------------------------------------
  // Methods for obtaining and modifying the position and size of the view.
  // Position is in the coordinate system of the view's parent.
  // Position is NOT flipped for RTL. See "RTL positioning" for RTL-sensitive
  // position accessors.
  // Transformations are not applied on the size/position. For example, if
  // bounds is (0, 0, 100, 100) and it is scaled by 0.5 along the X axis, the
  // width will still be 100 (although when painted, it will be 50x50, painted
  // at location (0, 0)).

  void SetBounds(int x, int y, int width, int height);
  void SetBoundsRect(const gfx::Rect& bounds);
  void SetSize(const gfx::Size& size);
  void SetPosition(const gfx::Point& position);
  void SetX(int x);
  void SetY(int y);

  // No transformation is applied on the size or the locations.
  const gfx::Rect& bounds() const { return bounds_; }
  int x() const { return bounds_.x(); }
  int y() const { return bounds_.y(); }
  int width() const { return bounds_.width(); }
  int height() const { return bounds_.height(); }
  const gfx::Size& size() const { return bounds_.size(); }

  // Returns the bounds of the content area of the view, i.e. the rectangle
  // enclosed by the view's border.
  gfx::Rect GetContentsBounds() const;

  // Returns the bounds of the view in its own coordinates (i.e. position is
  // 0, 0).
  gfx::Rect GetLocalBounds() const;

  // Returns the bounds of the layer in its own pixel coordinates.
  gfx::Rect GetLayerBoundsInPixel() const;

  // Returns the insets of the current border. If there is no border an empty
  // insets is returned.
  virtual gfx::Insets GetInsets() const;

  // Set whether this view is enabled. A disabled view does not receive keyboard
  // or mouse inputs. If |enabled| differs from the current value, SchedulePaint
  // is invoked. Also, clears focus if the focused view is disabled.
  void SetEnabled(bool enabled);

  // Returns whether the view is enabled.
  bool enabled() const { return enabled_; }

  // The background object is owned by this object and may be NULL.
  void set_background(Background* b);
  const Background* background() const { return background_.get(); }
  Background* background() { return background_.get(); }

  // The border object is owned by this object and may be NULL.
  virtual void SetBorder(scoped_ptr<Border> b);
  const Border* border() const { return border_.get(); }
  Border* border() { return border_.get(); }

  // focus
  virtual bool HasFocus() const;
  void SetFocusable(bool focusable);
  bool IsFocusable() const;

  // Called by the framework to paint a View. Performs translation and clipping
  // for View coordinates and language direction as required, allows the View
  // to paint itself via the various OnPaint*() event handlers and then paints
  // the hierarchy beneath it.
  virtual void Paint(gfx::Canvas* canvas, const CullSet& cull_set);

  static const char kViewClassName[];
  virtual const char* GetClassName() const;

  // Tree operations -----------------------------------------------------------
  int child_count() const { return static_cast<int>(children_.size()); }
  bool has_children() const { return !children_.empty(); }

  // Returns the child view at |index|.
  const View* child_at(int index) const {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, child_count());
    return children_[index];
  }
  View* child_at(int index) {
    return const_cast<View*>(const_cast<const View*>(this)->child_at(index));
  }

  // Returns the parent view.
  const View* parent() const { return parent_; }
  View* parent() { return parent_; }

  // Returns true if |view| is contained within this View's hierarchy, even as
  // an indirect descendant. Will return true if child is also this view.
  bool Contains(const View* view) const;
 protected:
  compositor::Layer* layer();
  const compositor::Layer* layer() const;

  // Overriden from widget::WidgetDelegate
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  bool CanFocus() override;
  bool OnCaptureLost() override;
  void OnPaint(gfx::Canvas* canvas) override;
  bool ShouldDescendIntoChildForEventHandling(widget::Widget* child,
                                              const gfx::Point& location) override;
  // Overridden from ui::EventHandler:
  void OnKeyEvent(ui::KeyEvent* event) override;
  void OnMouseEvent(ui::MouseEvent* event) override;
  void OnScrollEvent(ui::ScrollEvent* event) override;
  void OnTouchEvent(ui::TouchEvent* event) final;
  void OnGestureEvent(ui::GestureEvent* event) override;

  // Painting ------------------------------------------------------------------
  // Responsible for calling Paint() on child Views. Override to control the
  // order child Views are painted.
  virtual void PaintChildren(gfx::Canvas* canvas, const CullSet& cull_set);

  // Override to paint a background before any content is drawn. Typically this
  // is done if you are satisfied with a default OnPaint handler but wish to
  // supply a different background.
  virtual void OnPaintBackground(gfx::Canvas* canvas);

  // Override to paint a border not specified by SetBorder().
  virtual void OnPaintBorder(gfx::Canvas* canvas);

  View* parent_;
  Views children_;
  gfx::Rect bounds_;
  bool visible_;
  bool enabled_;
  bool focusable_;

  scoped_ptr<Background> background_;
  scoped_ptr<Border> border_;
  scoped_ptr<widget::Widget> widget_;
  DISALLOW_COPY_AND_ASSIGN(View);
};
}  // namespace views
}  // namespace azer
