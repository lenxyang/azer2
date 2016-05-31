#pragma once


#include "base/logging.h"
#include "azer/base/export.h"

#define ARRAY_SIZE(x) sizeof(x) / sizeof((x)[0])

namespace azer {
/**
 * InitApp 
 * 此函数负责初始化 //base 当中一些全局使用的变量
 * 此函数的最初实现是放在 source_set 当中的，后来改成了 dll-export 的形式
 * 这主要是由于 render 目前被设计成一个 dll， 但如果 chromium/baes 在 build
 * 过程中没有设置 is_component_build, 则 //base 模块会以静态库的形式存在于
 * 多个模块当中（至少包括render.dll 和 *.exe模块), 此时 InitApp 初始化的变量
 * render.dll 是无法使用的，因此必须将 InitApp 放置在 render.dll 模块当中才可以
 * 为了避免这种问题，于是采用了目前这种形式
 * 另外一个原因是使用 gn build chromium 时，如果使用 is_component_build = true,
 * 似乎会出现比较多的问题
 */
void AZER_EXPORT InitApp(int* argc, char** argv[], const char*);
}  // namespace azer

