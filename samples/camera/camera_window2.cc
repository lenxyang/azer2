#include <memory>
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "nelf/nelf.h"
#include "azer/render/render.h"
#include "azer/render/util.h"

using namespace azer;

class CameraOverlay;

class CameraView : public nelf::TextureRenderView {
 public:
  CameraOverlay(const Camera* camera);

  void Render() override;
 private:
  Camera overlay_camera_;
  scoped_ptr<CameraObject> object_;
};

CameraView::CameraView(const Camera* camera) {
  object_.reset(new CameraObject(camera));
  object_->GetTransformHolder()->SetPosition(Vector3(-0.0f, 0.0f, 0.0f));

  Vector3 camera_pos(2.0f, 2.0f, 2.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  overlay_camera_.reset(camera_pos, lookat, up);
}

void CameraView::Render() {
  Renderer* renderer = GetRenderer().get();
  object_->Update(overlay_camera_);
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  object_->Render(renderer);
}

class MainDelegate : public nelf::RenderDelegate {
 public:
  MainDelegate() 
      : prev_show_(0.0f),
        paint_view_(NULL) {
  }
  virtual bool Initialize() override;
  virtual void OnUpdate(const FrameArgs& args) override;
  virtual void OnRender(const FrameArgs& args) override;
 private:
  void InitUI();
  Camera camera_;
  DirLight light_;
  
  scoped_ptr<FPSCameraController> camera_controller_;
  scoped_ptr<CameraView> camera_view_;
  scoped_ptr<CoordinateGrid> gridline_;
  scoped_ptr<CoordinateObject> coord_object_;
  double prev_show_;
  CameraView* camera_view_;

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
  window()->SetRenderUI(true);

  coord_object_.reset(new CoordinateObject());
  camera_view_.reset(new CameraView(&camera_));
  gridline_.reset(new CoordinateGrid(1.0f, 1.0f, 100));
  gridline_->SetXCoordColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  gridline_->SetZCoordColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

  InitUI();
  return true;
}

void MainDelegate::InitUI() {
  nelf::SubWindow* window = new nelf::SubWindow(this->window());
  window->Init();
  window->GetWidget()->SetBounds(gfx::Rect(0, 0, 100, 180));
  window->SetLayoutManager(new views::FillLayout());

  camera_view_ = new CameraView(&camera_);
  window->AddChildView(camera_view_);
  window->Layout();
  window->Show();
}

void MainDelegate::OnUpdate(const FrameArgs& args) {
  camera_controller_->Update(args);
  gridline_->Update(camera_);
  coord_object_->Update(camera_);
  camera_view_->Render();
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

  if (args.time() - prev_show_ > 0.03) {
    camera_view_->UpdateUI();
    prev_show_ = args.time();
  }
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
