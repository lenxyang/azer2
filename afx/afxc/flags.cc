#include "azer/afx/afxc/flags.h"

#include <iostream>

#include "base/command_line.h"
#include "base/logging.h"

bool FLAGS_hlslang = false;
bool FLAGS_glslang = false;
bool FLAGS_list_hpp = false;
bool FLAGS_list_cpp = false;
bool FLAGS_list_afx = false;
bool FLAGS_list_effect = false;

base::FilePath::StringType FLAGS_afx;
base::FilePath::StringType FLAGS_afx_dir;
base::FilePath::StringType FLAGS_includes;
std::string FLAGS_output_dir;

void PrintHelp() {
  std::cout << "afxc: transloate afx to hlsl or glslang, and generate "
            << " c++ wrapper class " << std::endl
            << "  --afx" << std::endl
            << "  --afx_dir" << std::endl
            << "  --output_dir" << std::endl
            << "  --includes" << std::endl
            << "  --glslang: generate glslang code" << std::endl
            << "  --hlslang: generate hlslang code" << std::endl
            << "  --list_hpp: list hpp files generated" << std::endl
            << "  --list_cpp: list cpp files generated" << std::endl
            << "  --list_effect: list all effects" << std::endl;
}

int ParseArgs() {
  CommandLine* cmd = CommandLine::ForCurrentProcess();
  if (cmd->HasSwitch("help")) {
    PrintHelp();
    return -1;
  }

  FLAGS_afx = cmd->GetSwitchValueNative("afx");
  FLAGS_afx_dir = cmd->GetSwitchValueNative("afx_dir");
  FLAGS_output_dir = cmd->GetSwitchValueASCII("output_dir");
  FLAGS_includes = cmd->GetSwitchValueNative("includes");
  FLAGS_hlslang = cmd->HasSwitch("hlslang");
  FLAGS_glslang = cmd->HasSwitch("glslang");
  FLAGS_list_cpp = cmd->HasSwitch("list_cpp");
  FLAGS_list_hpp = cmd->HasSwitch("list_hpp");
  FLAGS_list_afx = cmd->HasSwitch("list_afx");
  FLAGS_list_effect = cmd->HasSwitch("list_effect");

  if (FLAGS_includes.empty()) {
    std::cerr << "includes cannot be empty\n";
    return -1;
  }

  if (FLAGS_afx.empty()) {
    std::cerr << "afx cannot be empty\n";
    return -1;
  }

  if (FLAGS_output_dir.empty()) {
    std::cerr << "output_dir cannot be empty\n";
    return -1;
  }

  return 0;
}
