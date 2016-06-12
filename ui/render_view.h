#pragma once

#include "ui/views/view.h"
#include "ui/views/widget/widget_observer.h"
#include "ui/views/focus/focus_manager.h"
#include "azer/render/texture.h"

namespace azer {

class EventListener;
class RenderSubWindow;
class RenderDelegate;
class RenderLoop;

class RenderView : public views::View,
                   public views::WidgetObserver,
                   public views::FocusChangeListener {
 public:
  static const char kViewClassName[];
  RenderView(RenderLoop* loop, RenderDelegate* delegate);
  ~RenderView() override;

  RenderSubWindow* window() { return window_;}
  void AddEventListener(EventListener* item);
  bool RemoveEventListener(EventListener* item);
  bool ContainsEventListener(EventListener* item);
  void SetSampleDesc(const azer::SampleDesc& desc);
  const azer::SampleDesc& sample_desc() const { return sample_desc_;}
  // views::View
  const char* GetClassName() const override;
  void OnFocus() override;
  void VisibilityChanged(View* starting_from, bool is_visible) override;;
  void OnBoundsChanged(const gfx::Rect& previous_bounds) override;
  void ViewHierarchyChanged(const ViewHierarchyChangedDetails& details) override;
  
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseMoved(const ui::MouseEvent& event) override;
  bool OnKeyPressed(const ui::KeyEvent& event) override;
  bool OnKeyReleased(const ui::KeyEvent& event) override;
  bool OnMouseWheel(const ui::MouseWheelEvent& event) override;
  void OnMouseCaptureLost() override;

  // override from views::FocusManager
  void OnWillChangeFocus(View* focused_before, View* focused_now) override;
  void OnDidChangeFocus(View* focused_before, View* focused_now) override;
protected:
  // override from views::WidgetObserver
  void OnWidgetDestroying(views::Widget* widget) override;
  void CreateRenderSubWindow();

  std::vector<EventListener*> listener_;
  RenderSubWindow* window_;
  RenderDelegate* delegate_;
  RenderLoop* loop_;
  azer::SampleDesc sample_desc_;
  DISALLOW_COPY_AND_ASSIGN(RenderView);
};
}  // namespace azer
