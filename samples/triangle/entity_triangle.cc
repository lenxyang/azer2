#include <memory>

#include "azer/azer.h"
#include "azer/ui/chromium_env.h"
#include "azer/ui/render_loop.h"
#include "azer/ui/sample_mainframe.h"
#include "azer/ui/window.h"
#include "azer/ui/desktop_window_context.h"
#include "azer/util/interactive/interactive.h"
#include "azer/util/geometry/geometry.h"
#include "azer/samples/triangle/simple_effect.h"

using base::FilePath;
using base::UTF8ToUTF16;
using namespace azer;

class MyRenderWindow : public azer::SampleMainframe {
 public:
  MyRenderWindow(const gfx::Rect& rect, WindowContext* ctx)
      : azer::SampleMainframe(rect, ctx) {}
  void OnInit() override;
  void OnUpdateFrame(const FrameArgs& args) override;
  void OnRenderFrame(const FrameArgs& args, Renderer* renderer) override;
 private:
  EntityPtr entity_;
  SimpleEffectPtr effect_;
  azer::Camera camera_;
  DISALLOW_COPY_AND_ASSIGN(MyRenderWindow);
};

int main(int argc, char* argv[]) {
  ChromiumEnv env(argc, argv);

  WindowContext* ctx = new DesktopWindowContext;
  ctx->Init(argc, argv);
  gfx::Rect init_bounds(0, 0, 800, 600);
  MyRenderWindow* window(new MyRenderWindow(init_bounds, ctx));
  SampleDesc sample_desc;
  sample_desc.count = 1;
  sample_desc.quality = 0;
  window->SetSampleDesc(sample_desc);
  window->SetCanResize(true);
  window->Init();
  window->Show();
  
  window->GetRenderLoop()->Run();
  return 0;
}

void MyRenderWindow::OnInit() {
  Vector3 camera_pos(0.0f, 0.0f, 5.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);

  effect_ = CreateSimpleEffect();
  Vector4 points[] = {Vector4( 0.0f, 1.0f, 0.0f, 1.0f),
                      Vector4(-1.0f, 0.0f, 0.0f, 1.0f),
                      Vector4( 1.0f, 0.0f, 0.0f, 1.0f)};
  VertexDataPtr vdata(new VertexData(effect_->vertex_desc(), arraysize(points)));
  VertexPack vpack(vdata.get());
  vpack.first();
  for (int32_t i = 0; i < arraysize(points); ++i) {
    vpack.WriteVector3Or4(points[i], VertexPos(0, 0));
    vpack.next(1);
  }
  IndicesDataPtr idata(new IndicesData(3));
  IndexPack ipack(idata.get());
  CHECK(ipack.WriteAndAdvance(0));
  CHECK(ipack.WriteAndAdvance(1));
  CHECK(ipack.WriteAndAdvance(2));
  EntityDataPtr edata(new EntityData(vdata.get(), idata.get()));
  edata->AddSubset(Subset(0, 3, 0, 3));
  entity_ = new Entity(edata.get());
}

void MyRenderWindow::OnUpdateFrame(const FrameArgs& args) {
}

void MyRenderWindow::OnRenderFrame(const FrameArgs& args, Renderer* renderer) {
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 1.0f, 1.0));
  renderer->ClearDepthAndStencil();

  effect_->SetPV(camera_.GetProjViewMatrix());
  effect_->SetWorld(Matrix4::kIdentity);
  effect_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  renderer->BindEffect(effect_.get());
  entity_->Draw(renderer);
}
