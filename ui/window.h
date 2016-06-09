#pragma once

#include <string>

#include "base/strings/string16.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

namespace azer {

class WindowContext;
class Window : public views::WidgetDelegateView {
 public:
  explicit Window(Window* parent);
  Window(const gfx::Rect& rect, WindowContext* ctx);
  Window(const gfx::Rect& rect, Window* parent);
  ~Window();

  static Window* GetWindow(views::View* view);
  WindowContext* context() { return context_;}
  void Init();
  void Show();
  void ShowWithCapture();
  views::Widget* GetWidget();

  // call this function must be called, before nonclient existed 
  void SetCanResize(bool value);
  void SetCanMinimize(bool value);
  void SetCanMaximize(bool value);
  void SetMinSize(const gfx::Size& size);
  void SetMaxSize(const gfx::Size& size);

  void SetTitle(const ::base::string16& title);
  void SetShowTitle(bool value);
  void SetWindowName(const std::string& name) { name_ = name;}
  void SetShowIcon(bool value);
  void SetWindowIcon(gfx::ImageSkia icon);
  void SetAppIcon(gfx::ImageSkia icon);
 protected:
  // override from views::View
  const char* GetClassName() const override;
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  bool CanResize() const override;
  bool CanMaximize() const override;
  bool CanMinimize() const override;
  base::string16 GetWindowTitle() const override;
  bool ShouldShowWindowTitle() const override;
  gfx::ImageSkia GetWindowIcon() override { return window_icon_;}
  gfx::ImageSkia GetWindowAppIcon() override { return app_icon_;}
  bool ShouldShowWindowIcon() const override;
  views::View* GetContentsView() override;
  void WindowClosing() override;
  void OnFocus() override;

  virtual void OnBeforeWidgetInit(views::Widget::InitParams* params,
                                  views::Widget* widget);
  virtual void OnAfterWidgetInit();
  virtual views::Widget* CreateWidget();
 private:
  void InitValue();
  ::base::string16 title_;
  bool show_title_;

  bool can_resize_;
  bool can_maximize_;
  bool can_minimize_;
  bool show_icon_;
  gfx::ImageSkia window_icon_;
  gfx::ImageSkia app_icon_;
  gfx::Size minsize_;
  gfx::Size maxsize_;
  gfx::Rect init_bounds_;
  WindowContext* context_;
  Window* parent_;
  std::string name_;
  DISALLOW_COPY_AND_ASSIGN(Window);
};
}  // namespace azer
