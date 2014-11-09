#include <iostream>
#include <fstream>

#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "azer/render/render.h"
#include "azer/afx/afxc/afx_wrapper.h"
#include "azer/afx/afxc/flags.h"

#include "base/command_line.h"
#include "base/at_exit.h"
#include "base/logging.h"
#include "base/i18n/icu_util.h"

using ::base::FilePath;
using azer::afx::TechniqueParser;
using azer::afx::AfxWrapper;

void GenCppCode(const AfxWrapper::AfxResult& result);
void GenHLSLTechniques(const AfxWrapper::AfxResult& result);
std::string stage_supfix(azer::RenderPipelineStage stage);
void WriteContent(const std::string& path, const std::string& content);

bool GenerateTechnique(const AfxWrapper::AfxResult& result) {
  const std::string& name = result.technique->name;
  return true;
}

int main(int argc, char* argv[]) {
  static ::base::AtExitManager at_exit;
  CommandLine::Init(*argc, *argv);
  ::logging::LoggingSettings setting;
  setting.log_file = TEXT("afxc.log");
  ::logging::InitLogging(setting);
  base::i18n::InitializeICU();

  if (0 != ParseArgs()) {
    return -1;
  }

  azer::afx::AfxWrapper afx(FLAGS_includes);
  azer::afx::AfxWrapperResultVec resvec;
  std::string errmsg;
  if (!afx.Parse(FilePath(FLAGS_afxpath), &errmsg, &resvec)) {
    std::cerr << errmsg << std::endl;
    return -1;
  }

  for (auto iter = resvec.begin(); iter != resvec.end(); ++iter) {
    const AfxWrapper::AfxResult& res = *iter;
    if (FLAGS_hlslang) {
      GenHLSLTechniques(res);
    }

    GenCppCode(res);
  }
  
  return 0;
}

void GenCppCode(const AfxWrapper::AfxResult& result) {
  std::string hpppath = ::base::StringPrintf("%s/%s.afx.h",
                                             FLAGS_output_dir.c_str(),
                                             FLAGS_cpp_filename.c_str());
  std::string cpppath = ::base::StringPrintf("%s/%s.afx.cc",
                                             FLAGS_output_dir.c_str(),
                                             FLAGS_cpp_filename.c_str());
  WriteContent(hpppath, result.hpp);
  WriteContent(cpppath, result.cpp);
}

void GenHLSLTechniques(const AfxWrapper::AfxResult& result) {
  for (int i = 0; i < azer::kRenderPipelineStageNum; ++i) {
    std::string code = result.hlsl[i];
    if (code.empty()) { continue; }
    azer::RenderPipelineStage stage = (azer::RenderPipelineStage)i;
    std::stringstream ss;
    ss << FLAGS_output_dir << "/" << result.technique->name << ".afx."
       << stage_supfix((azer::RenderPipelineStage)stage) << "";
    std::string path = ss.str();
    WriteContent(path, code);
  }
}

std::string stage_supfix(azer::RenderPipelineStage stage) {
  switch (stage) {
    case azer::kVertexStage: return "vs";
    case azer::kHullStage: return "hs";
    case azer::kTessellatorStage: return "ts";
    case azer::kDomainStage: return "ds";
    case azer::kGeometryStage: return "gs";
    case azer::kPixelStage: return "ps";
    default: NOTREACHED() << "No such RenderPipelineStage: " << stage; return "";
  }
}

void WriteContent(const std::string& path, const std::string& content) {
  ::base::FilePath afxpath(::base::UTF8ToWide(path));
  if (!PathExists(afxpath)) {
    if (::base::WriteFile(afxpath, content.c_str(), content.length())  == -1) {
      LOG(ERROR) << "failed to write file \"" << path << "\"";
    }
    return;
  }

  std::string tmp = path + ".afx.tmp";
  ::base::FilePath tmppath(::base::UTF8ToWide(tmp));
  if (::base::WriteFile(tmppath, content.c_str(), content.length())  == -1) {
    LOG(ERROR) << "failed to write file \"" << tmp << "\"";
  }

  if (TextContentsEqual(afxpath, tmppath)) {
    ::base::DeleteFile(tmppath, false);
  } else {
    ::base::File::Error error;
    if (!::base::ReplaceFile(tmppath, afxpath, &error)) {
      PLOG(ERROR) << "failed to replace file \"" << tmp << "\"";
    }
  }
}
