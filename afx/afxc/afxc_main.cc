#include <iostream>
#include <fstream>

#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/string_split.h"
#include "azer/afx/afxc/afx_wrapper.h"
#include "azer/afx/afxc/flags.h"
#include "azer/render/render.h"

#include "base/command_line.h"
#include "base/at_exit.h"
#include "base/logging.h"
#include "base/i18n/icu_util.h"

using ::base::FilePath;
using azer::afx::Technique;
using azer::afx::TechniqueLinker;
using azer::afx::AfxWrapper;

void GenCppCode(const AfxWrapper::AfxResult& result);
void GenHLSLTechniques(const AfxWrapper::AfxResult& result);
std::string stage_supfix(azer::RenderPipelineStage stage);
void WriteContent(const std::string& path, const std::string& content);

bool GenerateTechnique(const AfxWrapper::AfxResult& result) {
  const std::string& name = result.technique->name;
  return true;
}

void Init(int* argc, char** argv[]) {
  static ::base::AtExitManager at_exit;
  CommandLine::Init(*argc, *argv);
  ::logging::LoggingSettings setting;
  setting.log_file = TEXT("afxc.log");
  ::logging::InitLogging(setting);
  base::i18n::InitializeICU();
}

std::string ListFiles(const std::vector<AfxWrapper::AfxResult>& result);
void GenEffectFiles(const std::vector<AfxWrapper::AfxResult>& result);
std::string GetTechniqueFilePattern(const Technique& tech);

int main(int argc, char* argv[]) {
  Init(&argc, &argv);
  if (0 != ParseArgs()) {
    return -1;
  }

  std::vector<azer::afx::AfxWrapper*> afx;
  azer::afx::AfxWrapperResultVec resvec;
  std::vector< ::base::FilePath::StringType> files;
  ::base::SplitString(FLAGS_afxpath, AZER_LITERAL(','), &files);
  for (auto iter = files.begin(); iter != files.end(); ++iter) {
    std::string errmsg;
    azer::afx::AfxWrapper* wrapper = new azer::afx::AfxWrapper(FLAGS_includes);
    if (!wrapper->Parse(FilePath(*iter), &errmsg, &resvec)) {
      std::cerr << errmsg << std::endl;
      return -1;
    }
    afx.push_back(wrapper);
  }

  // list all files generated
  std::string allfiles = ListFiles(resvec);
  if (!allfiles.empty()) {
    std::cout << allfiles << std::endl;
    return 0;
  }

  GenEffectFiles(resvec);
  
  return 0;
}

void GenCppCode(const AfxWrapper::AfxResult& result) {
  // std::string pattern = GetTechniqueFilePattern(*result.technique);
  std::string pattern = FLAGS_cpp_filename;
  std::string hpppath = ::base::StringPrintf("%s/%s.afx.h",
                                             FLAGS_output_dir.c_str(),
                                             pattern.c_str());
  std::string cpppath = ::base::StringPrintf("%s/%s.afx.cc",
                                             FLAGS_output_dir.c_str(),
                                             pattern.c_str());
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

std::string ListFiles(const std::vector<AfxWrapper::AfxResult>& result) {
  std::stringstream ss;
  bool generated = false;
  for (auto iter = result.begin(); iter != result.end(); ++iter) {
    const AfxWrapper::AfxResult& res = *iter;
    if (FLAGS_list_cpp) {
      generated = true;
      ss << GetTechniqueFilePattern(*res.technique) << ".afx.cpp" << std::endl;
    } else if (FLAGS_list_hpp) {
      generated = true;
      ss << GetTechniqueFilePattern(*res.technique) << ".afx.hpp" << std::endl;
    } else if (FLAGS_list_afx) {
      generated = true;
      ss << GetTechniqueFilePattern(*res.technique) << ".afx" << std::endl;
    }
  }
  if (generated) {
    return ss.str();
  } else {
    return "";
  }
}

void GenEffectFiles(const std::vector<AfxWrapper::AfxResult>& resvec) {
  for (auto iter = resvec.begin(); iter != resvec.end(); ++iter) {
    const AfxWrapper::AfxResult& res = *iter;
    if (FLAGS_hlslang) {
      GenHLSLTechniques(res);
    }

    GenCppCode(res);
  }
}

std::string GetTechniqueFilePattern(const Technique& tech) {
  std::string pattern = tech.attributes->GetAttrValue("fpattern");
  if (pattern.empty()) {
    pattern = tech.name;
  }
  return pattern;
}
