#include "azer/afx/codegen/hlsl/afx_codegen.h"
#include "azer/afx/codegen/hlsl/ast_codegen.h"
#include "azer/afx/compiler/astdump.h"
#include "azer/afx/compiler/testutil.h"
#include "azer/afx/linker/technique_linker.h"
#include "azer/afx/linker/afx_linker.h"
#include "azer/afx/linker/afx_facade.h"
#include "azer/afx/linker/testutil.h"
#include "base/files/file_util.h"
#include "gtest/gtest.h"

using azer::afx::AfxLinker;
using azer::afx::AfxCodegen;
using azer::afx::ASTreeDumper;
using azer::afx::FileLoader;
using azer::afx::ParseContext;
using azer::afx::TechniqueLinker;

const std::string kTestdataDir = "azer/afx/testdata";
const std::string kAfxDir = "azer/afx/afx";

TEST(BuiltMaterial, Base) {
}
