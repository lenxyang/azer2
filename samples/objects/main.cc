#include <memory>
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "nelf/nelf.h"
#include "azer/render/render.h"
#include "azer/render/util.h"
#include "azer/samples/objects/effect_provider.h"

using namespace azer;

class MainDelegate : public nelf::RenderDelegate {
 public:
  MainDelegate() {}
  virtual bool Initialize() override;
  virtual void OnUpdate(const FrameArgs& args) override;
  virtual void OnRender(const FrameArgs& args) override;
 private:
  Camera camera_;
  DirLight light_;
  PVWEffectPtr pvw_effect_;
  ColoredDiffuseEffectPtr diffuse_effect_;
  GeometryObjectPtr sphere1_;
  EffectProviderPtr provider1_;

  GeometryObjectPtr sphere2_;
  EffectProviderPtr provider2_;

  GeometryObjectPtr box_;
  EffectProviderPtr box_provider_;

  GeometryObjectPtr cone_;
  EffectProviderPtr cone_provider_;
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

bool MainDelegate::Initialize() { 
  light_.dir = azer::Vector4(-0.4f, -0.4f, -0.2f, 0.0f);
  light_.diffuse = azer::Vector4(0.8f, 0.8f, 1.8f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  Vector3 camera_pos(0.0f, 10.0f, 10.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);

  RenderSystem* rs = RenderSystem::Current();
  pvw_effect_ = CreatePVWEffect();
  diffuse_effect_ = CreateColoredDiffuseEffect();
  
  sphere1_ = new SphereObject(pvw_effect_->GetVertexDesc(), 8, 8);
  provider1_ = new EffectProvider(&light_, &camera_);
  provider1_->GetTransformHolder()->SetPosition(Vector3(-3.0f, 0.0f, 0.0f));

  sphere2_ = new SphereObject(diffuse_effect_->GetVertexDesc(), 8, 8);
  provider2_ = new EffectProvider(&light_, &camera_);
  provider2_->GetTransformHolder()->SetPosition(Vector3(0.0f, 0.0f, 3.0f));

  box_ = new BoxObject(diffuse_effect_->GetVertexDesc());
  box_provider_ = new EffectProvider(&light_, &camera_);
  box_provider_->GetTransformHolder()->SetPosition(Vector3(3.0f, 0.0f, 3.0f));

  cone_ = new BoxObject(diffuse_effect_->GetVertexDesc());
  cone_provider_ = new EffectProvider(&light_, &camera_);
  cone_provider_->GetTransformHolder()->SetPosition(Vector3(3.0f, 0.0f, -3.0f));

  window()->SetRealTimeRender(true);
  return true;
}

void MainDelegate::OnUpdate(const FrameArgs& args) {
  Radians rad((3.14f) * (args.delta().InSecondsF()) * 0.2f);
  provider1_->GetTransformHolder()->rotate(Vector3(0.0f, 1.0f, 0.0f), rad);
  provider1_->UpdateParams(args);

  provider2_->GetTransformHolder()->rotate(Vector3(0.0f, 0.0f, 1.0f), rad);
  provider2_->UpdateParams(args);
  
  box_provider_->GetTransformHolder()->rotate(Vector3(1.0f, 0.0f, 0.0f), rad);
  box_provider_->UpdateParams(args);

  cone_provider_->GetTransformHolder()->rotate(Vector3(1.0f, 0.0f, 0.0f), rad);
  cone_provider_->UpdateParams(args);
}

void MainDelegate::OnRender(const FrameArgs& args) {
  RenderSystem* rs = RenderSystem::Current();
  Renderer* renderer = window()->GetRenderer().get();
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(kCullBack);
  renderer->EnableDepthTest(true);
  renderer->UseEffect(pvw_effect_.get());
  PVWEffectAdapter pvw_adapter;
  pvw_adapter.Apply(pvw_effect_.get(), provider1_.get());
  sphere1_->RenderWireframe(renderer);

  renderer->UseEffect(diffuse_effect_.get());
  ColoredEffectAdapter diffuse_adapter;
  diffuse_adapter.Apply(diffuse_effect_.get(), provider2_.get());
  sphere2_->Render(renderer);

  diffuse_adapter.Apply(diffuse_effect_.get(), box_provider_.get());
  box_->Render(renderer);

  // cone_provider_->Apply(diffuse_effect_.get());
  // cone_->Render(renderer);
}

int main(int argc, char* argv[]) {
  nelf::ContextPtr context = nelf::Context::CreateStandAloneContext(argc, argv);
  CHECK(context.get());

  // delegate and views must be create by "new"
  nelf::MainFrame* mainframe = new nelf::MainFrame(
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
