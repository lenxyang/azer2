#include <memory>

#include <iostream>
#include "base/files/file_util.h"

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

class TessEffect : public Effect {
 public:
  static const char kEffectName[];
  TessEffect() {
    world_ = Matrix4::kIdentity;
    color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  }
  ~TessEffect() {}

  const char* GetEffectName() const override { return kEffectName;}
#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pvw;
    Matrix4 world;
  };

  struct hs_cbuffer {
    Vector4 edge;
  };

  struct ps_cbuffer {
    Vector4 color;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value) { pv_ = value;}
  void SetWorld(const Matrix4& value) { world_ = value;}
  void SetColor(const Vector4& value) {color_ = value;}
  void SetEdge(const Vector4& value) {edge_ = value;}
 protected:
  void ApplyShaderParamTable(Renderer* renderer) override {
    Matrix4 pvw = std::move(pv_ * world_);
    {
      ShaderParamTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
      DCHECK(tb != NULL);
      tb->SetValue(0, &pvw, sizeof(Matrix4));
      tb->SetValue(1, &world_, sizeof(Matrix4));
    }
    {
      ShaderParamTable* tb = GetShaderClosure(kHullStage)->table_at(0);
      DCHECK(tb != NULL);
      tb->SetValue(0, &edge_, sizeof(Vector4));
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
    if (stage == kDomainStage) {
      closure->SetShader(shader, 0, 0, 0);
    } else if (stage == kVertexStage) {
      // generate GpuTable init for stage kVertexStage
      ShaderParamTable::Desc vs_table_desc[] = {
        ShaderParamTable::Desc("pvw", ShaderParamType::kMatrix4,
                                offsetof(vs_cbuffer, pvw), 1),
        ShaderParamTable::Desc("world", ShaderParamType::kMatrix4,
                                offsetof(vs_cbuffer, world), 1),
      };
      ShaderParamTablePtr table;
      table = new ShaderParamTable(arraysize(vs_table_desc), vs_table_desc);
      closure->SetShader(shader, 1, 0, 0);
      closure->SetShaderParamTable(0, table.get());
    } else if (stage == kHullStage) {

      // generate GpuTable init for stage kPixelStage
      ShaderParamTable::Desc hs_table_desc[] = {
        ShaderParamTable::Desc("edge", ShaderParamType::kVector4,
                                offsetof(hs_cbuffer, edge), 1),
      };
    
      table = new ShaderParamTable(arraysize(hs_table_desc), hs_table_desc);
      closure->SetShader(shader, 1, 0, 0);
      closure->SetShaderParamTable(0, table.get());
    } else if (stage == kPixelStage) {
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
  Vector4 edge_;
  DISALLOW_COPY_AND_ASSIGN(TessEffect);
};
const char TessEffect::kEffectName[] = "TessEffect";

typedef scoped_refptr<TessEffect> TessEffectPtr;
TessEffectPtr CreateTessEffect() {
  // class PositionVertex
  const VertexDesc::Desc kVertexDesc[] = {
    {"POSITION", 0, kVec4},
  };
  VertexDescPtr desc(new VertexDesc(kVertexDesc, arraysize(kVertexDesc)));
  TechSource tech(desc.get());
  const std::string path = "azer/samples/tessellation/tri.hlsl";
  CHECK(LoadShader(kVertexStage, path, &tech));
  CHECK(LoadShader(kHullStage, path, &tech));
  CHECK(LoadShader(kDomainStage, path, &tech));
  CHECK(LoadShader(kPixelStage, path, &tech));
  TessEffectPtr ptr(new TessEffect);
  ptr->Init(tech);
  return ptr;
}

class MyRenderWindow : public azer::SampleMainframe {
 public:
  MyRenderWindow(const gfx::Rect& rect, azer::WindowContext* ctx)
      : azer::SampleMainframe(rect, ctx) {}
  void OnInit() override;
  void OnUpdateFrame(const FrameArgs& args) override;
  void OnRenderFrame(const FrameArgs& args, Renderer* renderer) override;
 private:
  EntityPtr entity_;
  TessEffectPtr effect_;
  RasterizerStatePtr state_;
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
  Vector3 camera_pos(0.0f, 0.0f, 3.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  mutable_camera()->reset(camera_pos, lookat, up);

  effect_ = CreateTessEffect();
  Vector3 points[] = {Vector3( 0.0f,  0.51f, 0.0f),
                      Vector3(-0.5f, -0.21f, 0.0f),
                      Vector3( 0.5f, -0.21f, 0.0f)};
  entity_ = CreateGeoPointsList(kControlPoint3, points, (int)arraysize(points), 
                                effect_->vertex_desc(), Matrix4::kIdentity);

  state_ = RenderSystem::Current()->CreateRasterizerState();
  state_->SetFillMode(kWireFrame);
}

void MyRenderWindow::OnUpdateFrame(const FrameArgs& args) {
}

void MyRenderWindow::OnRenderFrame(const FrameArgs& args, Renderer* renderer) {
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 1.0f, 1.0));
  renderer->ClearDepthAndStencil();

  Vector3 position[] = {
    Vector3(-1.0f, -1.0f, 0.0f),
    Vector3(-1.0f,  1.0f, 0.0f),
    Vector3( 1.0f, -1.0f, 0.0f),
    Vector3( 1.0f,  1.0f, 0.0f),
  };

  Vector4 edge[] = {
    Vector4(3.0f, 3.0f, 3.0f, 1.0f),
    Vector4(3.0f, 3.0f, 3.0f, 3.0f),
    Vector4(3.0f, 3.0f, 3.0f, 4.0f),
    Vector4(3.0f, 5.0f, 6.0f, 6.0f),
  };

  effect_->SetPV(camera().GetProjViewMatrix());
  effect_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  for (uint32_t i = 0; i < arraysize(position); ++i) {
    effect_->SetWorld(Transform::Translate(position[i]));
    effect_->SetEdge(edge[i]);
    renderer->BindEffect(effect_.get());
    renderer->SetRasterizerState(state_.get());
    entity_->Draw(renderer);
  }
}
