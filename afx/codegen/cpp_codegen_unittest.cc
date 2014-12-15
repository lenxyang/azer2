#include "azer/afx/codegen/hlsl/afx_codegen.h"
#include "azer/afx/codegen/hlsl/ast_codegen.h"
#include "azer/afx/codegen/cpp_codegen.h"
#include "azer/afx/codegen/code_format.h"
#include "azer/afx/compiler/astdump.h"
#include "azer/afx/compiler/testutil.h"
#include "azer/afx/linker/technique_linker.h"
#include "azer/afx/linker/linker.h"
#include "azer/afx/linker/testutil.h"
#include "azer/afx/util/file_loader.h"
#include "base/files/file_util.h"
#include "gtest/gtest.h"

using azer::afx::AfxLinker;
using azer::afx::AfxCodegen;
using azer::afx::ASTreeDumper;
using azer::afx::FileLoader;
using azer::afx::ParseContext;
using azer::afx::Technique;
using azer::afx::TechniqueLinker;
using ::base::FilePath;

const ::base::FilePath::StringType kTestdataDir = AFXL("azer/afx/testdata");

namespace {
}  // namespace


TEST(AfxCppCodeGen, Sample) {
}

TEST(AfxCppCodeGen, Material) {
  std::vector<FilePath::StringType> inc;
  inc.push_back(kTestdataDir);
  AfxLinker::Options opt;
  opt.parse_astree = false;
  azer::afx::FileLoader loader(inc);
  AfxLinker linker(&loader, opt);
  std::string content;
  ASSERT_TRUE(azer::afx::LoadFileToString(
      ::base::FilePath(AZER_LITERAL("azer/afx/testdata/mtrl.afx")), &content));
  EXPECT_TRUE(linker.Load(content, AFXL("azer/afx/testdata/mtrl.afx")));

  LOG_IF(ERROR, !linker.success()) << linker.GetErrorText();
  // Dump(&linker);

  ParseContext* context = linker.root()->GetContext();
  DUMP_AFXCOMPILE_ERROR(*context);

  TechniqueLinker tlinker;
  EXPECT_TRUE(tlinker.Parse(context));
  Technique* technique = NULL;
  ASSERT_TRUE(tlinker.GetTechnique("effect", &technique));
  ASSERT_EQ(technique->name, "effect");
  const Technique::StageInfo& shader = technique->shader[azer::kPixelStage];
  ASSERT_TRUE(shader.entry != NULL);
  azer::afx::CppCodeGen cppcodegen;
  cppcodegen.GenCode(*technique);

  ::base::FilePath hpppath(AZER_LITERAL("azer/afx/testdata/cpp/mtrl.h"));
  ::base::FilePath cpppath(AZER_LITERAL("azer/afx/testdata/cpp/mtrl.cc"));
  ASSERT_TRUE(CompareCode(hpppath, cppcodegen.GetHeadCode()));
  ASSERT_TRUE(CompareCode(cpppath, cppcodegen.GetCppCode()));
}


TEST(AfxCppCodeGen, UniformArray) {
  std::vector<FilePath::StringType> inc;
  inc.push_back(kTestdataDir);
  AfxLinker::Options opt;
  opt.parse_astree = false;
  azer::afx::FileLoader loader(inc);
  AfxLinker linker(&loader, opt);
  std::string content;
  ASSERT_TRUE(azer::afx::LoadFileToString(
      ::base::FilePath(AZER_LITERAL("azer/afx/testdata/bone.afx")), &content));
  EXPECT_TRUE(linker.Load(content, AFXL("azer/afx/testdata/bone.afx")));

  LOG_IF(ERROR, !linker.success()) << linker.GetErrorText();
  // Dump(&linker);

  ParseContext* context = linker.root()->GetContext();
  DUMP_AFXCOMPILE_ERROR(*context);

  TechniqueLinker tlinker;
  EXPECT_TRUE(tlinker.Parse(context));
  Technique* technique = NULL;
  ASSERT_TRUE(tlinker.GetTechnique("haredware_skinned", &technique));
  ASSERT_EQ(technique->name, "haredware_skinned");
  const Technique::StageInfo& shader = technique->shader[azer::kPixelStage];
  ASSERT_TRUE(shader.entry != NULL);
  azer::afx::CppCodeGen cppcodegen;
  cppcodegen.GenCode(*technique);

  ::base::FilePath hpppath(AZER_LITERAL("azer/afx/testdata/cpp/bone.afx.h"));
  ::base::FilePath cpppath(AZER_LITERAL("azer/afx/testdata/cpp/bone.afx.cc"));
  ASSERT_TRUE(CompareCode(hpppath, cppcodegen.GetHeadCode()));
  ASSERT_TRUE(CompareCode(cpppath, cppcodegen.GetCppCode()));
}

TEST(AfxCppCodeGen, PointLight) {
  const std::string effect_name = "ptlight";
  const FilePath::StringType afx_path = AFXL("azer/afx/testdata/point_light.afx");
  std::vector<FilePath::StringType> inc;
  inc.push_back(kTestdataDir);
  AfxLinker::Options opt;
  opt.parse_astree = false;
  azer::afx::FileLoader loader(inc);
  AfxLinker linker(&loader, opt);
  std::string content;
  ASSERT_TRUE(azer::afx::LoadFileToString(::base::FilePath(afx_path), &content));
  EXPECT_TRUE(linker.Load(content, afx_path));

  LOG_IF(ERROR, !linker.success()) << linker.GetErrorText();
  // Dump(&linker);

  ParseContext* context = linker.root()->GetContext();
  DUMP_AFXCOMPILE_ERROR(*context);

  TechniqueLinker tlinker;
  EXPECT_TRUE(tlinker.Parse(context));
  Technique* technique = NULL;
  ASSERT_TRUE(tlinker.GetTechnique(effect_name, &technique));
  ASSERT_EQ(technique->name, effect_name);
  const Technique::StageInfo& shader = technique->shader[azer::kPixelStage];
  ASSERT_TRUE(shader.entry != NULL);
  azer::afx::CppCodeGen cppcodegen;
  cppcodegen.GenCode(*technique);

  ::base::FilePath hpppath(AZER_LITERAL("azer/afx/testdata/cpp/point_light.afx.h"));
  ::base::FilePath cpppath(AZER_LITERAL("azer/afx/testdata/cpp/point_light.afx.cc"));
  ASSERT_TRUE(CompareCode(hpppath, cppcodegen.GetHeadCode()));
  ASSERT_TRUE(CompareCode(cpppath, cppcodegen.GetCppCode()));
}

TEST(AfxCppCodeGen, TextureAdParam) {
  const std::string effect_name = "shadowmap";
  const FilePath::StringType afx_path = AFXL("azer/afx/testdata/shadowmap.afx");
  std::vector<FilePath::StringType> inc;
  inc.push_back(kTestdataDir);
  AfxLinker::Options opt;
  opt.parse_astree = false;
  azer::afx::FileLoader loader(inc);
  AfxLinker linker(&loader, opt);
  std::string content;
  ASSERT_TRUE(azer::afx::LoadFileToString(::base::FilePath(afx_path), &content));
  EXPECT_TRUE(linker.Load(content, afx_path));

  LOG_IF(ERROR, !linker.success()) << linker.GetErrorText();
  // Dump(&linker);

  ParseContext* context = linker.root()->GetContext();
  DUMP_AFXCOMPILE_ERROR(*context);

  TechniqueLinker tlinker;
  EXPECT_TRUE(tlinker.Parse(context));
  Technique* technique = NULL;
  ASSERT_TRUE(tlinker.GetTechnique(effect_name, &technique));
  ASSERT_EQ(technique->name, effect_name);
  const Technique::StageInfo& shader = technique->shader[azer::kPixelStage];
  ASSERT_TRUE(shader.entry != NULL);
  azer::afx::CppCodeGen cppcodegen;
  cppcodegen.GenCode(*technique);

  ::base::FilePath hpppath(AZER_LITERAL("azer/afx/testdata/cpp/shadowmap.afx.h"));
  ::base::FilePath cpppath(AZER_LITERAL("azer/afx/testdata/cpp/shadowmap.afx.cc"));
  ASSERT_TRUE(CompareCode(hpppath, cppcodegen.GetHeadCode()));
  ASSERT_TRUE(CompareCode(cpppath, cppcodegen.GetCppCode()));
}

