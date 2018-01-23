//===- String.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/JSON/String.h>
#include <cstring>

using namespace onnc;
using namespace onnc::json;

//===----------------------------------------------------------------------===//
// None member functions
//===----------------------------------------------------------------------===//
std::string onnc::json::trim(const char* pString)
{
  if (nullptr == pString)
    return std::string();

  size_t len = ::strlen(pString);
  char* result = (char*)malloc(sizeof(char)*len);
  size_t i = 0, c = 0;
  for (c = 0; c < len; ++c) {
    if ('\\' == pString[c]) {
      ++c;
      switch (pString[c]) {
        case '"':
        case '\\':
        case '/':
          result[i] = pString[c];
          break;
        case 'b':
          result[i] = '\b';
          break;
        case 'f':
          result[i] = '\f';
          break;
        case 'n':
          result[i] = '\n';
          break;
        case 'r':
          result[i] = '\r';
          break;
        case 't':
          result[i] = '\t';
          break;
      } // end of switch
    }
    else
      result[i] = pString[c];
    ++i;
  } // end of for loop

  // terminal of a C string.
  result[i] = 0;
  return std::string(result);
}
