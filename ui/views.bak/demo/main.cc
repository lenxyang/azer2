#include "azer/ui/views/util/init.h"
#include "azer/ui/views/widget/widget.h"
#include "azer/ui/aura/window_tree_host.h"

#include "azer/render/app_context.h"

int main(int argc, char* argv[]) {
  views::ViewsInitialize view_initializer(argc, argv);
  views::Widget::InitParams params;
  params.bounds = gfx::Rect(100, 100, 800, 600);
  views::Widget widget;
  widget.Init(params);
  widget.Show();

  aura::WindowTreeHost* host = widget.host();
  azer::AppContext* appcontext = azer::AppContext::GetInstance();
  CHECK(appcontext);
  while (true) {
    host->compositor()->DoComposite();

    base::MessageLoopForUI::current()->RunUntilIdle();
    azer::RendererPtr& renderer = appcontext->GetRenderer();
    renderer->Use();
    renderer->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    renderer->ClearDepthAndStencil();
    appcontext->RenderUI(host->compositor()->GetCompositedTexture());
    appcontext->GetSwapchain()->Present();
  }
  return 0;
}
