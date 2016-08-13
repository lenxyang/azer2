#include <memory>

#include "azer/azer.h"
#include "azer/ui/chromium_env.h"
#include "azer/ui/render_loop.h"
#include "azer/ui/sample_mainframe.h"
#include "azer/ui/window.h"
#include "azer/ui/desktop_window_context.h"
#include "azer/util/interactive/interactive.h"
#include "azer/util/geometry/geometry.h"

using base::FilePath;
using base::UTF8ToUTF16;
using namespace azer;

class SimpleEffect : public Effect {
 public:
  static const char kEffectName[];
  SimpleEffect() {}
  ~SimpleEffect() {}

  const char* GetEffectName() const override { return kEffectName;}
 protected:
  void ApplyShaderParamTable(Renderer* renderer) override {}
  ShaderClosurePtr InitVertexStage(Shader* shader) {
    ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
    closure->SetShader(shader, 0, 0, 0);
    return closure;
  }

  ShaderClosurePtr InitPixelStage(Shader* shader) {
    ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
    closure->SetShader(shader, 0, 0, 0);
    return closure;
  }
  DISALLOW_COPY_AND_ASSIGN(SimpleEffect);
};
const char SimpleEffect::kEffectName[] = "SimpleEffect";

typedef scoped_refptr<SimpleEffect> SimpleEffectPtr;
SimpleEffectPtr CreateSimpleEffect() {
  // class PositionVertex
  const VertexDesc::Desc kVertexDesc[] = {
    {"POSITION", 0, kVec4},
  };
  VertexDescPtr desc(new VertexDesc(kVertexDesc, arraysize(kVertexDesc)));
  TechSource tech(desc.get());
  ShaderInfo shader;
  shader.path = "effect.vs";
  shader.stage = kVertexStage;
  shader.code = ""
      "#pragma pack_matrix(row_major)\n"
      "struct VsOutput {\n"
      "  float4 position:SV_POSITION;\n"
      "}\n;"
      "struct VSInput {\n"
      "  float4 position:POSITION;\n"
      "};\n"
      "VsOutput vs_main(VSInput input) {\n"
      "VsOutput o;"
      "o.position = input.position;"
      "return o;"
      "}";
  tech.AddShader(shader);
  shader.path = "effect.ps";
  shader.stage = kPixelStage;
  shader.code = "#pragma pack_matrix(row_major)\n"
      "struct VsOutput {\n"
      "  float4 position:SV_POSITION;\n"
      "};\n"
      "float4 ps_main(VsOutput o):SV_TARGET {\n"
      "  return float4(1.0f, 0.0f, 0.0f, 1.0f);"
      "}\n";
  tech.AddShader(shader);
  SimpleEffectPtr ptr(new SimpleEffect);
  ptr->Init(tech);
  return ptr;
}

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
  effect_ = CreateSimpleEffect();
  Vector3 points[] = {Vector3( 0.0f, 1.0f, 0.0f),
                      Vector3(-1.0f, 0.0f, 0.0f),
                      Vector3( 1.0f, 0.0f, 0.0f)};
  entity_ = CreateGeoPointsList(kTriangleList, points, (int)arraysize(points), 
                                effect_->vertex_desc(), Matrix4::kIdentity);
}

void MyRenderWindow::OnUpdateFrame(const FrameArgs& args) {
}

void MyRenderWindow::OnRenderFrame(const FrameArgs& args, Renderer* renderer) {
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 1.0f, 1.0));
  renderer->ClearDepthAndStencil();
  renderer->BindEffect(effect_.get());
  entity_->Draw(renderer);
}
