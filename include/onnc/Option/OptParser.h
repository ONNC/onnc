//===- OptParser.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_OPTION_PARSER_H
#define ONNC_OPTION_OPTION_PARSER_H
#include <onnc/ADT/TriState.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Option/OptValue.h>
#include <onnc/Support/DataTypes.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <string>

namespace onnc {
namespace cl {

class OptDefs;

class BasicOptParser
{
public:
  virtual ~BasicOptParser() { }
};

/** \class OptParser
 *  \brief The generic parser is used to parse enumeration type.
 */
template<class DataType>
class OptParser : public BasicOptParser
{
public:
  typedef DataType value_type;
  typedef OptValue<DataType> ValueType;
  typedef StringMap<ValueType> ValueMap;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             DataType& pResult) {
    typename ValueMap::iterator entry = m_ValueMap.find(pValue);
    if (m_ValueMap.end() == entry)
      return false;
    pResult = entry->value().getValue();
    return true;
  }

  void addOptValue(const ValueType& pValue) {
    bool exist = false;
    typename ValueMap::entry_type* entry =
                                    m_ValueMap.insert(pValue.getName(), exist);
    if (exist) {
      fatal(opt_multi_enum) << pValue.getName();
      return;
    }

    entry->setValue(pValue);
  }

private:
  ValueMap m_ValueMap;
};

//===----------------------------------------------------------------------===//
// OptParser<bool>
//===----------------------------------------------------------------------===//
template<>
class OptParser<bool> : public BasicOptParser
{
public:
  typedef bool value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             bool& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<TriValue>
//===----------------------------------------------------------------------===//
template<>
class OptParser<TriState> : public BasicOptParser
{
public:
  typedef TriState value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             TriState& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<char>
//===----------------------------------------------------------------------===//
template<>
class OptParser<char> : public BasicOptParser
{
public:
  typedef char value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             char& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<int>
//===----------------------------------------------------------------------===//
template<>
class OptParser<int> : public BasicOptParser
{
public:
  typedef int value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             int& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<unsigned int>
//===----------------------------------------------------------------------===//
template<>
class OptParser<unsigned int> : public BasicOptParser
{
public:
  typedef unsigned int value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             unsigned int& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<long long>
//===----------------------------------------------------------------------===//
template<>
class OptParser<long long> : public BasicOptParser
{
public:
  typedef long long value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             long long& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<unsigned long long>
//===----------------------------------------------------------------------===//
template<>
class OptParser<unsigned long long> : public BasicOptParser
{
public:
  typedef unsigned long long value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             unsigned long long& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<double>
//===----------------------------------------------------------------------===//
template<>
class OptParser<double> : public BasicOptParser
{
public:
  typedef double value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             double& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<float>
//===----------------------------------------------------------------------===//
template<>
class OptParser<float> : public BasicOptParser
{
public:
  typedef float value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             float& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<std::string>
//===----------------------------------------------------------------------===//
template<>
class OptParser<std::string> : public BasicOptParser
{
public:
  typedef std::string value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             std::string& pResult);
};

//===----------------------------------------------------------------------===//
// OptParser<Path>
//===----------------------------------------------------------------------===//
template<>
class OptParser<Path> : public BasicOptParser
{
public:
  typedef Path value_type;

public:
  bool parse(const OptDefs& pOption, StringRef pArg, StringRef pValue,
             Path& pResult);
};

} // namespace of cl
} // namespace of onnc

#endif
