#include <memory>
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "nelf/nelf.h"
#include "azer/render/render.h"
#include "azer/render/util.h"

using namespace azer;

class CameraOverlay {
 public:
  CameraOverlay(const Camera* camera);

  void Update();
  void Render(Renderer* renderer);
 private:
  OverlayPtr overlay_;
  Camera overlay_camera_;
  RendererPtr texrenderer_;
  scoped_ptr<CameraObject> object_;
  Renderer::Viewport viewport_;
};

CameraOverlay::CameraOverlay(const Camera* camera) {
  RenderSystem* rs = RenderSystem::Current();
  azer::Texture::Options rdopt;
  rdopt.size = gfx::Size(400, 400);
  rdopt.target = (azer::Texture::BindTarget)
      (azer::Texture::kRenderTarget | azer::Texture::kShaderResource);
  texrenderer_ = rs->CreateRenderer(rdopt);
  viewport_.bounds = gfx::Rect(rdopt.size);
  texrenderer_->SetViewport(viewport_);

  overlay_ = rs->CreateOverlay();
  overlay_->SetBounds(gfx::RectF(0.5f, 0.5f, 0.5f, 0.5f));
  overlay_->SetTexCoord(gfx::PointF(0.0f, 0.0f), gfx::PointF(1.0f, 1.0f));

  object_.reset(new CameraObject(camera));
  object_->GetTransformHolder()->SetPosition(Vector3(-0.0f, 0.0f, 0.0f));

  Vector3 camera_pos(2.0f, 2.0f, 2.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  overlay_camera_.reset(camera_pos, lookat, up);
}

void CameraOverlay::Update() {
  Renderer* texrd = texrenderer_.get();
  object_->Update(overlay_camera_);
  texrd->Use();
  texrd->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  texrd->ClearDepthAndStencil();
  object_->Render(texrd);
}

void CameraOverlay::Render(Renderer* renderer) {
  overlay_->SetTexture(texrenderer_->GetRenderTarget()->GetTexture());
  overlay_->Render(renderer);
}

class MainDelegate : public nelf::RenderDelegate {
 public:
  MainDelegate() {}
  virtual bool Initialize() override;
  virtual void OnUpdate(const FrameArgs& args) override;
  virtual void OnRender(const FrameArgs& args) override;
 private:
  Camera camera_;
  DirLight light_;
  
  scoped_ptr<FPSCameraController> camera_controller_;
  scoped_ptr<CameraOverlay> camera_overlay_;
  scoped_ptr<CoordinateGrid> gridline_;
  scoped_ptr<CoordinateObject> coord_object_;
  
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

bool MainDelegate::Initialize() { 
  light_.dir = Vector4(-0.6f, -0.2f, -0.2f, 0.0f);
  light_.diffuse = Vector4(0.8f, 0.8f, 1.8f, 1.0f);
  light_.ambient = Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  Vector3 camera_pos(0.0f, 1.0f, 5.0f);
  Vector3 lookat(0.0f, 1.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);
  camera_controller_.reset(new FPSCameraController(&camera_));
  view()->AddEventListener(camera_controller_.get());
  window()->SetRealTimeRender(true);

  coord_object_.reset(new CoordinateObject());
  camera_overlay_.reset(new CameraOverlay(&camera_));
  gridline_.reset(new CoordinateGrid(1.0f, 1.0f, 100));
  gridline_->SetXCoordColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  gridline_->SetZCoordColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  return true;
}

void MainDelegate::OnUpdate(const FrameArgs& args) {
  camera_controller_->Update(args);
  camera_overlay_->Update();
  gridline_->Update(camera_);
  coord_object_->Update(camera_);
}

void MainDelegate::OnRender(const FrameArgs& args) {
  Renderer* renderer = window()->GetRenderer().get();
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(kCullBack);
  renderer->EnableDepthTest(true);
  gridline_->Render(renderer);
  coord_object_->Render(renderer);
  camera_overlay_->Render(renderer);
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