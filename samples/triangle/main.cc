#include <memory>

#include "base/strings/utf_string_conversions.h"
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "nelf/nelf.h"
#include "azer/render/render.h"
#include "azer/render/util.h"
#include "azer/samples/triangle/diffuse.afx.h"

using base::FilePath;
using base::FilePath;
using base::UTF8ToUTF16;

using namespace azer;
class MainDelegate : public nelf::RenderDelegate {
 public:
  MainDelegate(nelf::ContextPtr context) : standalone_context_(context) {}
  virtual bool Initialize() override;
  virtual void OnUpdate(const FrameArgs& args) override;
  virtual void OnRender(const FrameArgs& args) override;
 private:
  VertexBufferPtr vb_;
  scoped_refptr<DiffuseEffect> effect_;
  Matrix4 world_;
  Camera camera_;
  nelf::ContextPtr standalone_context_;
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

bool MainDelegate::Initialize() {
  RenderSystem* rs = RenderSystem::Current();
  Effect::ShaderPrograms shaders;
  CHECK(LoadShaderAtStage(kVertexStage, "azer/samples/triangle/diffuse.afx.vs", &shaders));
  CHECK(LoadShaderAtStage(kPixelStage, "azer/samples/triangle/diffuse.afx.ps", &shaders));
  effect_ = new DiffuseEffect();
  effect_->Init(shaders);
  DiffuseEffect::Vertex v[] = {
    DiffuseEffect::Vertex(Vector4( 0.0f, 1.0f, 0.0f, 1.0f ),
                          Vector4( 1.0f, 0.0f, 0.0f, 1.0f )),
    DiffuseEffect::Vertex(Vector4( -1.0f, -1.0f, 0.0f, 1.0f ),
                          Vector4( 0.0f, 1.0f,  0.0f, 1.0f )),
    DiffuseEffect::Vertex(Vector4( 1.0f, -1.0f, 0.0f, 1.0f ),
                          Vector4( 0.0f, 0.0f, 1.0f, 1.0f )),
  };
  VertexDataPtr vdata(new VertexData(effect_->GetVertexDesc(), ARRAYSIZE(v)));
  memcpy(vdata->pointer(), (uint8*)v, sizeof(v));
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata.get());

  Vector3 pos(0.0f, 0.0f, 5.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(pos, lookat, up);
  window()->SetRealTimeRender(true);
  window()->SetRenderUI(true);
  nelf::SubWindow* window = new nelf::SubWindow(this->window());
  window->Init();
  window->GetWidget()->SetBounds(gfx::Rect(10, 10, 200, 300));
  window->SetLayoutManager(new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 3));
  window->Show();
  views::Label* label = new views::Label(UTF8ToUTF16("Label"));
  label->SetBounds(10, 0, 100, 30);
  window->AddChildView(label);
  views::Checkbox* checkbox = new views::Checkbox(UTF8ToUTF16("Wireframe"));
  checkbox->SetBounds(10, 40, 100, 30);
  window->AddChildView(checkbox);
  return true;
}

void MainDelegate::OnUpdate(const FrameArgs& args) {
  world_ = RotateY(Radians(args.time()));
}

void MainDelegate::OnRender(const FrameArgs& args) {
  Matrix4 pvw = std::move(camera_.GetProjViewMatrix() * world_);
  RenderSystem* rs = RenderSystem::Current();
  RendererPtr renderer = window()->GetRenderer();
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(kCullNone);
  effect_->SetPVW(pvw);
  effect_->Use(renderer.get());
  renderer->Draw(vb_.get(), kTriangleList, 3, 0);
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
  MainDelegate azer_content(context);
  nelf::RenderView* render_view = new nelf::RenderView(render_loop.get(), 
                                                       &azer_content);
  render_loop->AddRenderView(render_view);
  mainframe->SetLayoutManager(new views::FillLayout);
  mainframe->AddChildView(render_view);
  mainframe->LayoutContents();
  render_loop->Run();
  return 0;
}
