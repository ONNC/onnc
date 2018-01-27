//===-- OptParser.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Option/OptParser.h>
#include <onnc/ADT/StringRef.h>

using namespace onnc;
using namespace onnc::cl;

//===----------------------------------------------------------------------===//
// OptParser<bool>
//===----------------------------------------------------------------------===//
bool OptParser<bool>::parse(const OptDefs& pOption,
                            StringRef pArg,
                            StringRef pValue,
                            bool& pResult)
{
  if (pValue == "" || pValue == "true" || pValue == "TRUE" || pValue == "True" ||
      pValue == "1") {
    pResult = true;
    return true;
  }

  if (pValue == "false" || pValue == "FALSE" || pValue == "False" || pValue == "0") {
    pResult = false;
    return true;
  }
  error(pOption, "invalid value for boolean argument! Try true or false");
  return false;
}

//===----------------------------------------------------------------------===//
// OptParser<TriState>
//===----------------------------------------------------------------------===//
bool OptParser<TriState>::parse(const OptDefs& pOption,
                                StringRef pArg,
                                StringRef pValue,
                                TriState& pResult)
{
  if (pValue == "" || pValue == "true" || pValue == "TRUE" || pValue == "True" ||
      pValue == "1") {
    pResult = TriTrue;
    return true;
  }

  if (pValue == "false" || pValue == "FALSE" || pValue == "False" || pValue == "0") {
    pResult = TriFalse;
    return true;
  }
  error(pOption, "invalid value for boolean argument! Try true or false");
  return false;
}

//===----------------------------------------------------------------------===//
// OptParser<char>
//===----------------------------------------------------------------------===//
bool OptParser<char>::parse(const OptDefs& pOption,
                            StringRef pArg,
                            StringRef pValue,
                            char& pResult)
{
  pResult = pValue[0];
  return true;
}
//===----------------------------------------------------------------------===//
// OptParser<int>
//===----------------------------------------------------------------------===//
bool OptParser<int>::parse(const OptDefs& pOption,
                           StringRef pArg,
                           StringRef pValue,
                           int& pResult)
{
  long long result = 0;
  if (!StrToLongLong(pValue, 0, result)) {
    error(pOption, "invalid value for integer argument!");
    return false;
  }
  pResult = result;
  return true;
}
//===----------------------------------------------------------------------===//
// OptParser<unsigned int>
//===----------------------------------------------------------------------===//
bool OptParser<unsigned int>::parse(const OptDefs& pOption,
                              StringRef pArg,
                              StringRef pValue,
                              unsigned int& pResult)
{
  unsigned long long result = 0;
  if (!StrToULongLong(pValue, 0, result)) {
    error(pOption, "invalid value for unsigned integer argument!");
    return false;
  }
  pResult = result;
  return true;
}

//===----------------------------------------------------------------------===//
// OptParser<long long>
//===----------------------------------------------------------------------===//
bool OptParser<long long>::parse(const OptDefs& pOption,
                                 StringRef pArg,
                                 StringRef pValue,
                                 long long& pResult)
{
  long long result = 0;
  if (!StrToLongLong(pValue, 0, result)) {
    error(pOption, "invalid value for unsigned integer argument!");
    return false;
  }
  pResult = result;
  return true;
}

//===----------------------------------------------------------------------===//
// OptParser<unsigned long long>
//===----------------------------------------------------------------------===//
bool OptParser<unsigned long long>::parse(const OptDefs& pOption,
                                    StringRef pArg,
                                    StringRef pValue,
                                    unsigned long long& pResult)
{
  unsigned long long result = 0;
  if (!StrToULongLong(pValue, 0, result)) {
    error(pOption, "invalid value for unsigned integer argument!");
    return false;
  }
  pResult = result;
  return true;
}

//===----------------------------------------------------------------------===//
// OptParser<double>
//===----------------------------------------------------------------------===//
bool OptParser<double>::parse(const OptDefs& pOption,
                            StringRef pArg,
                            StringRef pValue,
                            double& pResult)
{
  const char* arg_start = pValue.data();
  char* end;
  double value = strtod(arg_start, &end);
  if (0 != *end) {
    error(pOption, "invalid value for double floating point argument!");
    return false;
  }
  pResult = value;
  return true;
}

//===----------------------------------------------------------------------===//
// OptParser<float>
//===----------------------------------------------------------------------===//
bool OptParser<float>::parse(const OptDefs& pOption,
                            StringRef pArg,
                            StringRef pValue,
                            float& pResult)
{
  const char* arg_start = pValue.data();
  char* end;
  double value = strtod(arg_start, &end);
  if (0 != *end) {
    error(pOption, "invalid value for floating point argument!");
    return false;
  }
  pResult = (float)value;
  return true;
}

//===----------------------------------------------------------------------===//
// OptParser<std::string>
//===----------------------------------------------------------------------===//
bool OptParser<std::string>::parse(const OptDefs& pOption,
                                   StringRef pArg,
                                   StringRef pValue,
                                   std::string& pResult)
{
  pResult = pValue.str();
  return true;
}

//===----------------------------------------------------------------------===//
// OptParser<Path>
//===----------------------------------------------------------------------===//
bool OptParser<Path>::parse(const OptDefs& pOption,
                            StringRef pArg,
                            StringRef pValue,
                            Path& pResult)
{
  pResult = pValue.str();
  return true;
}
