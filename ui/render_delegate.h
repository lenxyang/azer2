#pragma once

#include "azer/render/frame_args.h"
#include "azer/render/render_system.h"
#include "azer/render/renderer.h"

namespace azer {

class RenderView;
class RenderWindow;
class RenderDelegate {
 public:
  // attention the widget must be TopLevelWidget
  RenderDelegate() : window_(NULL), view_(NULL) {}
  virtual ~RenderDelegate() {}

  virtual bool Initialize() = 0;
  virtual void OnUpdate(const azer::FrameArgs& frame_data) = 0;
  virtual void OnRender(const azer::FrameArgs& frame_data) = 0;

  RenderWindow* window() { return window_;}
  RenderView* view() { return view_;}
 protected:
  RenderWindow* window_;
  RenderView* view_;
  friend RenderWindow;
  DISALLOW_COPY_AND_ASSIGN(RenderDelegate);
};

}  // namespace azer
