#include <memory>
#include "base/memory/ref_counted.h"
#include "base/files/file_util.h"
#include "nelf/nelf.h"
#include "azer/base/image.h"
#include "azer/render/render.h"
#include "azer/render/util.h"
#include "azer/samples/vertex_buffer/share_texcoord/indepeffect.afx.h"

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
  IndepDiffuseEffectPtr effect_;
  VertexBufferGroupPtr plane1_, plane2_;
  TexturePtr tex1_, tex2_;
  IndicesBufferPtr ib_;
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

VertexBufferPtr InitVertexBuffer(VertexDescPtr desc, float posx, float posz, 
                                 float step = 1.0f) {
  int kLineCount = 10 + 1;
  SlotVertexDataPtr vdata(new SlotVertexData(desc, kLineCount * kLineCount));
  VertexPack vpack(vdata.get());
  vpack.first();
  float xstep = 1.0f / (float)10.0f;
  float zstep = 1.0f / (float)10.0f;
  for (int32 i = 0; i < kLineCount; ++i) {
    for (int32 j = 0; j < kLineCount; ++j) {
      float x = posx + step * j;
      float z = posz + step * i;
      float u = j * xstep;
      float v = i * zstep;
      vpack.WriteVector4(Vector4(x, 0.0f, z, 1.0f), VertexPos(0, 0)); 
      vpack.WriteVector2(Vector2(u, v), VertexPos(0, 1)); 
      vpack.next(1);
    }
  }

  RenderSystem* rs = RenderSystem::Current();
  VertexBufferPtr vb = rs->CreateVertexBuffer(VertexBuffer::Options(), vdata);
  return vb;
}

IndicesBufferPtr CreatePlaneIndices(int row, int column) {
  const int32 kIndexNum = (row - 1) * (column - 1) * 2 * 3;
  IndicesDataPtr idata(new IndicesData(kIndexNum));
  IndexPack ipack(idata.get());
  for (int i = 0; i < row - 1; ++i) {
    for (int j = 0; j < column - 1; ++j) {
      int cur_line = i * column;
      int next_line = (i + 1) * column;
      CHECK(ipack.WriteAndAdvance(cur_line  + j));
      CHECK(ipack.WriteAndAdvance(next_line + j));
      CHECK(ipack.WriteAndAdvance(next_line + j + 1));
      CHECK(ipack.WriteAndAdvance(cur_line  + j));
      CHECK(ipack.WriteAndAdvance(next_line + j + 1));
      CHECK(ipack.WriteAndAdvance(cur_line  + j + 1));
    }
  }
  RenderSystem* rs = RenderSystem::Current();
  return rs->CreateIndicesBuffer(IndicesBuffer::Options(), idata);
}


bool MainDelegate::Initialize() { 
  light_.dir = azer::Vector4(-0.6f, -0.6f, -0.2f, 0.0f);
  light_.diffuse = azer::Vector4(0.8f, 0.8f, 1.8f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  Vector3 camera_pos(0.0f, 30.0f, 5.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);

  RenderSystem* rs = RenderSystem::Current();
  effect_ = CreateDiffuse();
  VertexBufferPtr vb1 = InitVertexBuffer(effect_->GetVertexDesc(), -10.0f, -10.0f);
  VertexBufferPtr vb2 = InitVertexBuffer(effect_->GetVertexDesc(), 0.0f, 0.0f);
  ib_ = CreatePlaneIndices(11, 11);
  plane1_ = rs->CreateVertexBufferGroup(effect_->GetVertexDesc());
  plane2_ = rs->CreateVertexBufferGroup(effect_->GetVertexDesc());
  plane1_->add_vertex_buffer_at(vb1, 0);
  plane2_->add_vertex_buffer_at(vb2, 0);

  scoped_ptr<Image> img1(Image::Load(::base::FilePath(
      FILE_PATH_LITERAL("azer/samples/media/texture/WoodCrate01.dds")), Image::k2D));
  scoped_ptr<Image> img2(Image::Load(::base::FilePath(
      FILE_PATH_LITERAL("azer/samples/media/texture/WoodCrate02.dds")), Image::k2D));
  Texture::Options opt;
  opt.target = Texture::kShaderResource;
  tex1_ = rs->CreateTexture(opt, img1.get());
  tex2_ = rs->CreateTexture(opt, img2.get());
  return true;
}

void MainDelegate::OnUpdate(const FrameArgs& args) {
  Radians rad((3.14f) * (args.delta().InSecondsF()) * 0.2f);
}

void MainDelegate::OnRender(const FrameArgs& args) {
  RenderSystem* rs = RenderSystem::Current();
  Renderer* renderer = window()->GetRenderer().get();
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(kCullNone);
  renderer->EnableDepthTest(true);
  
  effect_->SetPVW(camera_.GetProjViewMatrix());
  effect_->SetTexture(tex1_);
  effect_->Use(renderer);
  ib_->Use(renderer);
  plane1_->Use(renderer);
  renderer->SetPrimitiveTopology(kTriangleList);
  renderer->DrawIndex(ib_->indices_num(), 0);

  effect_->SetTexture(tex2_);
  effect_->Use(renderer);
  ib_->Use(renderer);
  plane2_->Use(renderer);
  renderer->DrawIndex(ib_->indices_num(), 0);
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
