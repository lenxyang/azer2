#include <memory>
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "nelf/nelf.h"
#include "azer/render/render.h"
#include "azer/render/util.h"
#include "azer/samples/mesh/effect_provider2.h"

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
  MeshPtr mesh_;
  EffectAdapterContext context_;
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

bool MainDelegate::Initialize() { 
  light_.dir = azer::Vector4(-0.6f, -0.6f, -0.2f, 0.0f);
  light_.diffuse = azer::Vector4(0.8f, 0.8f, 1.8f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  context_.RegisteAdapter(typeid(ColoredDiffuseEffect), 
                          typeid(EnvironmentEffectProvider), 
                          new EnvironmentEffectAdapter);

  Vector3 camera_pos(0.0f, 3.0f, 5.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);

  RenderSystem* rs = RenderSystem::Current();
  ColoredDiffuseEffectPtr diffuse_effect = CreateColoredDiffuseEffect();
  GeometryObjectPtr obj = new CylinderObject(diffuse_effect->GetVertexDesc());
  EnvironmentEffectProviderPtr env_provider(
      new EnvironmentEffectProvider(&light_, &camera_));
  env_provider->GetTransformHolder()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
  mesh_ = new Mesh(&context_);
  mesh_->AddCommonProvider(env_provider);

  Mesh::Entity entity;
  entity.effect = diffuse_effect;
  entity.vb = obj->GetVertexBuffer();
  entity.ib = obj->GetIndicesBuffer();
  entity.provider = new MaterialEffectProvider();
  entity.adapter = new MaterialEffectAdapter;
  mesh_->AddEntity(entity);

  window()->SetRealTimeRender(true);
  return true;
}

void MainDelegate::OnUpdate(const FrameArgs& args) {
  mesh_->Update(args);
}

void MainDelegate::OnRender(const FrameArgs& args) {
  RenderSystem* rs = RenderSystem::Current();
  Renderer* renderer = window()->GetRenderer().get();
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(kCullNone);
  renderer->EnableDepthTest(true);
  mesh_->DrawIndex(renderer, azer::kTriangleList);
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
