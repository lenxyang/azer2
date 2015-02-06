#include "azer/ui/views/util/init.h"
#include "azer/ui/views/widget/widget.h"

int main(int argc, char* argv[]) {
  views::ViewsInitialize view_initializer(argc, argv);
  views::InitParams params;
  params.bounds = gfx::Rect(100, 100, 800, 600);
  views::Widget widget;
  widget.Init(params);
  return 0;
}
