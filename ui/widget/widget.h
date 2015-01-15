#pragma once

#include <string>
#include <vector>

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

class Widget : public compositor::LayerDelegate {
 public:
  typedef std::vector<Widget*> Widgets;
  enum WidgetType {
    kRoot,
    kNoDraw,
    kCanvas,
    k3DRenderer,
    kBitmap,
  };

  explicit Widget(WidgetType type);
  virtual ~Widget();

  void SetName(const std::string& name);
  const std::string& name() const;
  WidgetType type() const { return layer_type_;}
  
  void SetDelegate(WidgetDelegate* delegate);

  Widget* parent() { return parent_;}
  const Widget* parent() const { return parent_;}
  void AddChild(Widget* widget);
  void RemoveChild(Widget* widget);
  bool Contains(const Widget* widget) const;
  const Widgets& children() const { return children_;}
 protected:
  // compositor::LayerDelegate
  void OnPaintLayer(gfx::Canvas* canvas) override;
  void InitLayer();
  compositor::Layer* CreateLayerByType() const;

  int64 id_;
  WidgetTreeHost* host_;
  Widget* parent_;
  Widgets children_;
  WidgetDelegate* delegate_;

  WidgetType layer_type_;
  scoped_ptr<compositor::Layer> layer_;
  
  gfx::Rect bounds_;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};

}  // namespace widget
}  // namespace azer


