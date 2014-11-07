#pragma once

#include <ostream>
#include <string.h>
#include "azer/base/build_info.h"
#include "base/logging.h"


#define LEX_TRACE \
  ((azer::afx::ParseContext*)context)->tokenizer_stream() << "[Tokenizer] "
#define PARSER_TRACE \
  ((azer::afx::ParseContext*)context)->parser_stream()  \
  << azer::GetRelativePath(__FILE__)                  \
  << ":" << __LINE__  << " [Parser] "

#define PRE_PARSER_TRACE \
  (parseContext)->parser_stream()                               \
  << azer::GetRelativePath(__FILE__)                            \
  << ":" << __LINE__  << " [Parser] "
