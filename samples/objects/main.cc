#include <memory>
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "nelf/nelf.h"
#include "azer/render/render.h"
#include "azer/render/util.h"

using namespace azer;

class EffectProvider : public EffectParamsProvider {
 public:
  EffectProvider(const DirLight* light) : light_(light) {}
  void Update(const Camera& camera) {
    world_ = std::move(holder_.GenWorldMatrix());
    pvw_ = std::move(camera.GetProjViewMatrix() * world_);
  }

  void Apply(Effect* e) {
    if (typeid(*e) == typeid(PVWEffect)) {
      PVWEffect* effect = dynamic_cast<PVWEffect*>(e);
      effect->SetWorld(world_);
      effect->SetPVW(pvw_);
    } else if (typeid(*e) == typeid(ColoredDiffuseEffect)) {
      ColoredDiffuseEffect* effect = dynamic_cast<ColoredDiffuseEffect*>(e);
      effect->SetWorld(world_);
      effect->SetPVW(pvw_);
      effect->SetDirLight(light_);
      effect->SetColor(Vector4(1.0, 0.0, 0.0, 1.0));
    }
  }

  TransformHolder* GetTransformHolder() { return &holder_;}
  const TransformHolder* GetTransformHolder() const { return &holder_;}
 private:
  const DirLight* light_;
  Matrix4 world_;
  Matrix4 pvw_;
  TransformHolder holder_;
};

typedef scoped_ptr<EffectProvider> EffectProviderPtr;

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
  GeometryObjectPtr sphere2_;
  EffectProviderPtr provider1_;
  EffectProviderPtr provider2_;
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

bool MainDelegate::Initialize() { 
  light_.dir = azer::Vector4(0.0f, -0.3f, -0.5f, 0.0f);
  light_.diffuse = azer::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  Vector3 camera_pos(0.0f, 5.0f, 5.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);

  RenderSystem* rs = RenderSystem::Current();
  pvw_effect_ = CreatePVWEffect();
  diffuse_effect_ = ColoredDiffuseEffect();
  sphere1_ = new SphereObject(pvw_effect_->GetVertexDesc(), 8, 8);
  sphere2_ = new SphereObject(diffuse_effect_->GetVertexDesc(), 8, 8);
  
  provider1_.reset(new EffectProvider(&light_));
  provider2_.reset(new EffectProvider(&light_));
  provider1_->GetTransformHolder()->SetPosition(Vector3(-3.0f, 0.0f, 0.0f));
  provider2_->GetTransformHolder()->SetPosition(Vector3(3.0f, 0.0f, 0.0f));

  window()->SetRealTimeRender(true);
  return true;
}

void MainDelegate::OnUpdate(const FrameArgs& args) {
  Radians rad((3.14f) * (args.delta().InSecondsF()) * 0.2f);
  provider1_->GetTransformHolder()->rotate(Vector3(0.0f, 1.0f, 0.0f), rad);
  provider2_->GetTransformHolder()->rotate(Vector3(0.0f, 0.0f, 1.0f), rad);
  provider1_->Update(camera_);
  provider2_->Update(camera_);
}

void MainDelegate::OnRender(const FrameArgs& args) {
  RenderSystem* rs = RenderSystem::Current();
  Renderer* renderer = window()->GetRenderer().get();
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(kCullNone);
  pvw_effect_->Use(renderer);
  provider1_->Apply(pvw_effect_.get());
  sphere1_->RenderWireframe(renderer);

  diffuse_effect_->Use(renderer);
  provider2_->Apply(diffuse_effect_.get());
  sphere2_->Render(renderer);
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
