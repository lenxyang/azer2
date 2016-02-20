#pragma once

#include <string>
#include "base/files/file_path.h"

extern bool FLAGS_hlslang;
extern bool FLAGS_glslang;
extern bool FLAGS_list_hpp;
extern bool FLAGS_list_cpp;
extern bool FLAGS_list_afx;
extern bool FLAGS_list_effect;
extern base::FilePath::StringType FLAGS_afx;
extern base::FilePath::StringType FLAGS_afx_dir;
extern base::FilePath::StringType FLAGS_includes;
extern std::string FLAGS_output_dir;
extern std::string FLAGS_cpp_filename;

int ParseArgs();
void PrintHelp();
