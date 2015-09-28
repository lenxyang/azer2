#include <memory>
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "nelf/nelf.h"
#include "azer/render/render.h"
#include "azer/render/util/util.h"

using azer::GeometryObjectPtr;

class MainDelegate : public nelf::RenderDelegate {
 public:
  MainDelegate() {}
  virtual bool Initialize() override;
  virtual void OnUpdate(const azer::FrameArgs& args) override;
  virtual void OnRender(const azer::FrameArgs& args) override;
 private:
  azer::PVWEffectPtr effect_;
  azer::Camera camera_;
  GeometryObjectPtr box1_;
  GeometryObjectPtr box2_;
  GeometryObjectPtr sphere1_;
  GeometryObjectPtr sphere2_;
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

bool MainDelegate::Initialize() { 
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  azer::Effect::ShaderPrograms shaders;
  CHECK(wow::LoadShaderAtStage(azer::kVertexStage,
                               "wow/sandbox/render/object/diffuse.hlsl.vs",
                               &shaders));
  CHECK(wow::LoadShaderAtStage(azer::kPixelStage,
                               "wow/sandbox/render/object/diffuse.hlsl.ps",
                               &shaders));
  effect_ = new azer::PVWEffect(azer::PositionVertex::CreateVertexDesc());
  effect_->Init(shaders);
  box1_ = new azer::SphereObject(effect_, 8, 8);
  box2_ = new azer::BoxObject(effect_);
  // box1_->GetTransformHolder()->SetPosition(azer::Vector3(-3.0f, 0.0f, 0.0f));

  azer::Vector3 camera_pos(0.0f, 5.0f, 5.0f);
  azer::Vector3 lookat(0.0f, 0.0f, 0.0f);
  azer::Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);
  window()->SetRealTimeRender(true);
  return true;
}

void MainDelegate::OnUpdate(const azer::FrameArgs& args) {
  azer::Radians rad((3.14f) * (args.delta().InSecondsF()) * 0.2f);
  box1_->GetTransformHolder()->rotate(azer::Vector3(0.0f, 1.0f, 0.0f), rad);
  box2_->GetTransformHolder()->rotate(azer::Vector3(0.0f, 0.0f, 1.0f), rad);
  box1_->Update(camera_);
  box2_->Update(camera_);
}

void MainDelegate::OnRender(const azer::FrameArgs& args) {
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  azer::RendererPtr renderer = window()->GetRenderer();
  renderer->Use();
  renderer->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(azer::kCullNone);
  box1_->RenderWireframe(renderer.get());
  // box1_->Render(renderer.get());
  // box2_->Render(renderer.get());
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
