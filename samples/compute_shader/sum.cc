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

class MyRenderWindow : public azer::SampleMainframe {
 public:
  MyRenderWindow(const gfx::Rect& rect, WindowContext* ctx) 
      : azer::SampleMainframe(rect, ctx) {}
  void OnInit() override;
  void OnUpdateFrame(const FrameArgs& args) override;
  void OnRenderFrame(const FrameArgs& args, Renderer* renderer) override;
 private:
  azer::StructuredGpuBufferPtr input1_, input2_, output_;
  azer::GpuBufferPtr toread_;
  azer::ShaderResViewPtr input1_view_, input2_view_;
  azer::UnorderAccessViewPtr output_view_;
  scoped_refptr<GpuComputeTask> compute_task_;
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
  RenderSystem* rs = RenderSystem::Current();
  output_ = new StructuredGpuBuffer(4 * 4, (int)sizeof(float));
  GpuResOptions struct_opt;
  struct_opt.type = GpuResType::kCommonBuffer;
  struct_opt.cpu_access = kCPURead;
  struct_opt.usage = kBufferStaging;
  struct_opt.target = 0;
  toread_ = rs->CreateBuffer(struct_opt, 4 * 4, (int)sizeof(float), NULL);

  // 
  std::unique_ptr<float> init_data(new float[64 * 64]);
  for (int i = 0; i < 64; ++i) {
    for (int j = 0; j < 64; ++j) {
      (init_data.get())[i * 64 + j] = 0.1 * i + 0.1f * j;
    }
  }
  input1_ = new StructuredGpuBuffer(64 * 64, (int)sizeof(float), 
                                    (uint8_t*)init_data.get());
  input2_ = new StructuredGpuBuffer(64 * 64, (int)sizeof(float), 
                                    (uint8_t*)init_data.get());

  input1_view_ = rs->CreateBufferShaderResView(input1_->gpu_buffer());
  input2_view_ = rs->CreateBufferShaderResView(input2_->gpu_buffer());
  output_view_ = rs->CreateBufferUAView(output_->gpu_buffer());

  ShaderInfo shader;
  shader.path = "azer/samples/compute_shader/sum.cs.hlsl";
  shader.stage = kComputeStage;
  CHECK(::base::ReadFileToString(FilePath(UTF8ToUTF16(shader.path)), &shader.code));
  compute_task_ = new GpuComputeTask(shader, 0, 2, 1);
  compute_task_->SetResource(0, input1_view_.get());
  compute_task_->SetResource(1, input2_view_.get());
  compute_task_->SetUAResource(0, output_view_.get());
}

void MyRenderWindow::OnUpdateFrame(const FrameArgs& args) {
}

void MyRenderWindow::OnRenderFrame(const FrameArgs& args, Renderer* renderer) {
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 1.0f, 1.0));
  renderer->ClearDepthAndStencil();

  static bool computed = false;
  if (!computed) {
    computed = false;
    compute_task_->Bind(renderer);
    GpuTaskParams params;
    params.thread_group.x = 1;
    params.thread_group.y = 1;
    params.thread_group.z = 1;
    renderer->DispatchComputeTask(params);

    output_->gpu_buffer()->CopyTo(toread_.get());
    GpuResLockDataPtr lockdata = toread_->map(MapType::kReadOnly);
    float* data = (float*)lockdata->data_ptr();
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        std::cout << *data << "\t";
        data++;
      }
      std::cout << std::endl;
    }
    toread_->unmap();
  }
}
