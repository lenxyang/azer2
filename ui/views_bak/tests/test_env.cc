#include "azer/ui/views/tests/test_env.h"

#include "base/message_loop/message_loop.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "ui/base/ime/input_method_initializer.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/win/scoped_ole_initializer.h"
#include "ui/base/ui_base_paths.h"
#include "ui/wm/core/wm_state.h"
#include "ui/gfx/win/dpi.h"

#include "azer/render/util/render_system_loader.h"

namespace views {
namespace test {

TestEnv::TestEnv(int argc, char* argv[]) {
#if defined(OS_WIN)
  gfx::InitDeviceScaleFactor(1.0f);
#endif
  ole_initializer_.reset(new ui::ScopedOleInitializer);
  CHECK(azer::LoadRenderSystem());

  ui::RegisterPathProvider();
  base::FilePath ui_test_pak_path;
  DCHECK(PathService::Get(ui::UI_TEST_PAK, &ui_test_pak_path));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(ui_test_pak_path);
}

TestEnv::~TestEnv() {
}

}  // namespace test
}  // namespace views
