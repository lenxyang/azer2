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
  SimpleEffect() {
    world_ = Matrix4::kIdentity;
    color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  }
  ~SimpleEffect() {}

  const char* GetEffectName() const override { return kEffectName;}

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pvw;
    Matrix4 world;
  };

  struct ps_cbuffer {
    Vector4 color;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value) { pv_ = value;}
  void SetWorld(const Matrix4& value) { world_ = value;}
  void SetColor(const Vector4& value) {color_ = value;}
 protected:
  void ApplyShaderParamTable(Renderer* renderer) override {
    {
      Matrix4 pvw = std::move(pv_ * world_);
      ShaderParamTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
      DCHECK(tb != NULL);
      tb->SetValue(0, &pvw, sizeof(Matrix4));
      tb->SetValue(1, &world_, sizeof(Matrix4));
    }
    {
      ShaderParamTable* tb = GetShaderClosure(kPixelStage)->table_at(0);
      DCHECK(tb != NULL);
      tb->SetValue(0, &color_, sizeof(Vector4));
    }
  }

  ShaderClosurePtr InitShaderClosure(RenderPipelineStage stage, Shader* shader) {
    ShaderClosurePtr closure(new ShaderClosure(stage));
    ShaderParamTablePtr table;
    if (stage == kVertexStage) {
      // generate GpuTable init for stage kVertexStage
      ShaderParamTable::Desc vs_table_desc[] = {
        ShaderParamTable::Desc("pvw", ShaderParamType::kMatrix4,
                               offsetof(vs_cbuffer, pvw), 1),
        ShaderParamTable::Desc("world", ShaderParamType::kMatrix4,
                               offsetof(vs_cbuffer, world), 1),
      };

      table = new ShaderParamTable(arraysize(vs_table_desc), vs_table_desc);
      closure->SetShader(shader, 1, 0, 0);
      closure->SetShaderParamTable(0, table.get());
    } else if(stage == kPixelStage) {
      // generate GpuTable init for stage kPixelStage
      ShaderParamTable::Desc ps_table_desc[] = {
        ShaderParamTable::Desc("color", ShaderParamType::kVector4,
                               offsetof(ps_cbuffer, color), 1),
      };

      table = new ShaderParamTable(arraysize(ps_table_desc), ps_table_desc);
      closure->SetShader(shader, 1, 0, 0);
      closure->SetShaderParamTable(0, table.get());
    } else {
      CHECK(false);
    }
    return closure;
  }

  Matrix4 pv_;
  Matrix4 world_;
  Vector4 color_;
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
      "cbuffer c_buffer {\n"
      "  float4x4 pvw;"
      "  float4x4 world;"
      "};"
      "VsOutput vs_main(VSInput input) {\n"
      "VsOutput o;"
      "o.position = mul(pvw, input.position);"
      "return o;"
      "}";
  tech.AddShader(shader);
  shader.path = "effect.ps";
  shader.stage = kPixelStage;
  shader.code = "#pragma pack_matrix(row_major)\n"
      "struct VsOutput {\n"
      "  float4 position:SV_POSITION;\n"
      "};\n"
      "cbuffer c_buffer {\n"
      "  float4 color;\n"
      "};\n"
      "float4 ps_main(VsOutput o):SV_TARGET {\n"
      "  return color;"
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
  SimpleEffectPtr effect_;
  azer::Camera camera_;
  VertexBufferPtr vb_;
  IndexBufferPtr ib_;
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
  Vector3 camera_pos(0.0f, 0.0f, 5.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_.reset(camera_pos, lookat, up);

  effect_ = CreateSimpleEffect();
  Vector4 points[] = {Vector4( 0.0f, 1.0f, 0.0f, 1.0f),
                      Vector4(-1.0f, 0.0f, 0.0f, 1.0f),
                      Vector4( 1.0f, 0.0f, 0.0f, 1.0f)};
  VertexDataPtr vdata(new VertexData(effect_->vertex_desc(), arraysize(points)));
  VertexPack vpack(vdata.get());
  vpack.first();
  for (int32_t i = 0; i < arraysize(points); ++i) {
    vpack.WriteVector3Or4(points[i], VertexPos(0, 0));
    vpack.next(1);
  }
  IndicesDataPtr idata(new IndicesData(3));
  IndexPack ipack(idata.get());
  CHECK(ipack.WriteAndAdvance(0));
  CHECK(ipack.WriteAndAdvance(1));
  CHECK(ipack.WriteAndAdvance(2));
  EntityDataPtr edata(new EntityData(vdata.get(), idata.get()));
  edata->AddSubset(Subset(0, 3, 0, 3));
  entity_ = new Entity(edata.get());
}

void MyRenderWindow::OnUpdateFrame(const FrameArgs& args) {
}

void MyRenderWindow::OnRenderFrame(const FrameArgs& args, Renderer* renderer) {
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 1.0f, 1.0));
  renderer->ClearDepthAndStencil();

  effect_->SetPV(camera_.GetProjViewMatrix());
  effect_->SetWorld(Matrix4::kIdentity);
  effect_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  renderer->BindEffect(effect_.get());
  entity_->Draw(renderer);
}
