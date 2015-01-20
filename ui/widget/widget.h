#pragma once

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "ui/events/event_target.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/base/export.h"
#include "azer/ui/compositor/layer_delegate.h"


namespace gfx {
class Canvas;
}  // namespace gfx

namespace azer {

namespace compositor {
class Layer;
}  // namespace compositor

namespace widget {

class WidgetDelegate;
class WidgetTreeHost;

class AZER_EXPORT Widget : public compositor::LayerDelegate 
                         , public ::ui::EventTarget {
 public:
  typedef std::vector<Widget*> Widgets;
  enum WidgetType {
    kRoot,
    kNoDraw,
    kCanvas,
    k3DRenderer,
    kBitmap,
  };

  Widget(WidgetType type, Widget* parent);
  explicit Widget(Widget* parent);
  explicit Widget(WidgetTreeHost* host);
  virtual ~Widget();

  Widget* root();
  compositor::Layer* layer();
  const compositor::Layer* layer() const;

  void SetName(const std::string& name);
  const std::string& name() const;
  WidgetType type() const { return layer_type_;}

  void SetVisible(bool visible) { visible_ = visible;}
  bool IsVisible() const { return visible_;}

  void SetBounds(const gfx::Rect& bounds);
  const gfx::Rect& bounds() const { return bounds_;}

  void set_ignore_events(bool ignore) { ignore_events_ = ignore;}
  bool ignore_events() const { return ignore_events_;}
  
  void SetDelegate(WidgetDelegate* delegate);
  WidgetDelegate* delegate() { return delegate_;}

  // Sets a new event-targeter for the window, and returns the previous
  // event-targeter.
  scoped_ptr<ui::EventTargeter> SetEventTargeter(
      scoped_ptr<ui::EventTargeter> targeter);

  Widget* parent() { return parent_;}
  const Widget* parent() const { return parent_;}
  void AddChild(Widget* widget);
  void RemoveChild(Widget* widget);
  bool Contains(const Widget* widget) const;
  const Widgets& children() const { return children_;}

  void SchedulePaint();
  void SchedulePaintInRect(const gfx::Rect& r);

  // Converts |point| from |source|'s coordinates to |target|'s. If |source| is
  // NULL, the function returns without modifying |point|. |target| cannot be
  // NULL.
  static void ConvertPointToTarget(const Widget* source,
                                   const Widget* target,
                                   gfx::Point* point);
  static void ConvertRectToTarget(const Widget* source,
                                  const Widget* target,
                                  gfx::Rect* rect);
  // Overridden from ui::EventTarget:
 protected:
  bool CanAcceptEvent(const ui::Event& event) override;
  EventTarget* GetParentTarget() override;
  scoped_ptr<ui::EventTargetIterator> GetChildIterator() const override;
  ui::EventTargeter* GetEventTargeter() override;
 public:
  // access by WidgetTargeter
  void ConvertEventToTarget(ui::EventTarget* target,
                            ui::LocatedEvent* event) override;
 protected:
  // compositor::LayerDelegate
  void OnPaintLayer(gfx::Canvas* canvas) override;
  void InitLayer();
  compositor::Layer* CreateLayerByType();

  bool IsRootWidget() { return parent_ == NULL;}

  int64 id_;
  WidgetTreeHost* host_;
  Widget* parent_;
  Widgets children_;
  WidgetDelegate* delegate_;
  bool ignore_events_;
  bool visible_;

  WidgetType layer_type_;
  compositor::Layer* layer_;
  scoped_ptr<ui::EventTargeter> targeter_;
  
  gfx::Rect bounds_;

  friend class WidgetTreeHost;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};

}  // namespace widget
}  // namespace azer


