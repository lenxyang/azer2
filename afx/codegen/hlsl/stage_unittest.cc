#include "azer/afx/afx.h"
#include "azer/afx/compiler/testutil.h"
#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "gtest/gtest.h"

using azer::afx::AfxFacade;
using azer::afx::AfxLinker;
using azer::afx::HLSLPSAfxCodegen;
using azer::afx::HLSLVSAfxCodegen;
using azer::afx::HLSLGSAfxCodegen;
using azer::afx::ASTreeDumper;
using azer::afx::FileLoader;
using azer::afx::ParseContext;
using azer::afx::Technique;
using azer::afx::TechniqueLinker;
using base::FilePath;

const FilePath::StringType kTestdataDir = AFXL("azer/afx/testdata");

TEST(StageUnittest, GeometryStage) {
  const std::string expect = ""
      "// using row_major\n"
      "#pragma pack_matrix(row_major)\n\n"
      "struct VSOutput {float3 position:SV_POSITION;"
      "float3 normal:NORMAL;"
      "float2 texcoord:TEXCOORD;"
      "};"
      "[maxvertexcount(18)]\n"
      "void gsmain(VSOutput v,inout LineStream<VSOutput> gs_ostream){"
      "VSOutput nv;"
      "gs_ostream.Append(nv);"
      "gs_ostream.RestartStrip();}";
  std::vector<FilePath::StringType> inc;
  inc.push_back(kTestdataDir);
  AfxLinker::Options opt;
  opt.parse_astree = false;
  AfxFacade parser(inc, opt);
  ASSERT_TRUE(parser.Parse(AFXL("azer/afx/testdata/gs.afx")));

  LOG_IF(ERROR, !parser.success()) << parser.GetErrorText();
  ParseContext* context = parser.GetContext();
  DUMP_AFXCOMPILE_ERROR(*context);
  Technique* technique = parser.GetTechnique("effect");
  ASSERT_TRUE(NULL != technique);
  ASSERT_EQ(technique->name, "effect");
  Technique::StageInfo& shader = technique->shader[azer::kGeometryStage];
  ASSERT_TRUE(shader.entry != NULL);

  HLSLGSAfxCodegen codegen;
  ASSERT_EQ(codegen.GenCode(shader), expect);
  // LOG(ERROR) << codegen.GenCode(shader);
}

