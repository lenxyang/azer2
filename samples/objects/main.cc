#include <memory>
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "nelf/nelf.h"
#include "azer/render/render.h"
#include "azer/render/util.h"

using namespace azer;

class MainDelegate : public nelf::RenderDelegate {
 public:
  MainDelegate() {}
  virtual bool Initialize() override;
  virtual void OnUpdate(const FrameArgs& args) override;
  virtual void OnRender(const FrameArgs& args) override;
 private:
  PVWEffectPtr pvw_effect_;
  ColoredDiffuseEffectPtr diffuse_effect_;
  Camera camera_;
  GeometryObjectPtr sphere1_;
  GeometryObjectPtr sphere2_;
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

bool MainDelegate::Initialize() { 
  RenderSystem* rs = RenderSystem::Current();
  pvw_effect_ = CreatePVWEffect();
  diffuse_effect_ = ColoredDiffuseEffect();
  sphere1_ = new SphereObject(pvw_effect_, 8, 8);
  sphere2_ = new SphereObject(diffuse_effect_, 8, 8);
  
  sphere1_->GetTransformHolder()->SetPosition(Vector3(-3.0f, 0.0f, 0.0f));
  sphere2_->GetTransformHolder()->SetPosition(Vector3(3.0f, 0.0f, 0.0f));

  Vector3 camera_pos(0.0f, 5.0f, 5.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);
  window()->SetRealTimeRender(true);
  return true;
}

void MainDelegate::OnUpdate(const FrameArgs& args) {
  Radians rad((3.14f) * (args.delta().InSecondsF()) * 0.2f);
  sphere1_->GetTransformHolder()->rotate(Vector3(0.0f, 1.0f, 0.0f), rad);
  sphere2_->GetTransformHolder()->rotate(Vector3(0.0f, 0.0f, 1.0f), rad);
  sphere1_->Update(camera_);
  sphere2_->Update(camera_);
}

void MainDelegate::OnRender(const FrameArgs& args) {
  RenderSystem* rs = RenderSystem::Current();
  RendererPtr renderer = window()->GetRenderer();
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(kCullNone);
  sphere1_->RenderWireframe(renderer.get());
  sphere2_->Render(renderer.get());
}

int main(int argc, char* argv[]) {
  nelf::ContextPtr context = nelf::Context::CreateStandAloneContext(argc, argv);
  CHECK(context.get());

  // delegate and views must be create by "new"
  nelf::Mainframe* mainframe = new nelf::Mainframe(
      gfx::Rect(100, 100, 800, 600), context.get());
  mainframe->Init();
  mainframe->Show();

  scoped_refptr<nelf::RenderLoop> render_loop(new nelf::RenderLoop(mainframe));
  MainDelegate azer_content;
  nelf::RenderView* render_view = new nelf::RenderView(render_loop.get(), 
                                                       &azer_content);
  render_loop->AddRenderView(render_view);
  mainframe->SetLayoutManager(new views::FillLayout);
  mainframe->AddChildView(render_view);
  mainframe->LayoutContents();
  render_loop->Run();
  return 0;
}
