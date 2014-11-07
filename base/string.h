#pragma once

#include <ostream>
#include "azer/base/render_export.h"
#include "base/strings/utf_string_conversions.h"
#include "base/files/file_path.h"

namespace azer {

#if defined(OS_POSIX)
// On most platforms, native pathnames are char arrays, and the encoding
// may or may not be specified.  On Mac OS X, native pathnames are encoded
// in UTF-8.
typedef std::string StringType;
#elif defined(OS_WIN)
// On Windows, for Unicode-aware applications, native pathnames are wchar_t
// arrays encoded in UTF-16.
typedef std::wstring StringType;
#endif  // OS_WIN

typedef StringType::value_type CharType;

// Macros for string literal initialization of FilePath::CharType[], and for
// using a FilePath::CharType[] in a printf-style format string.
#if defined(OS_POSIX)
#define AZER_LITERAL(x) x
#elif defined(OS_WIN)
#define AZER_LITERAL(x) L ## x
#endif  // OS_WIN

template<class T>
void ConvertPath(const char *path, T*);

template<>
inline void ConvertPath<std::string>(const char *path, std::string* str) {
  *str = path;
}

template<>
inline void ConvertPath<std::wstring>(const char *path, std::wstring* str) {
  *str = std::wstring(::base::UTF8ToWide(path));
}
}  // namespace azer

AZER_EXPORT extern std::ostream& operator << (std::ostream& os,
                                              const ::base::FilePath& path);




