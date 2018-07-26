//===- OptValue.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_OPTION_VALUE_H
#define ONNC_OPTION_OPTION_VALUE_H
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/StringMap.h>
#include <onnc/Config/AboutData.h>
#include <onnc/Option/OptDefs.h>

namespace onnc {
namespace cl {

/** \class OptValue
 *  \brief OptValue describe the value of an option.
 */
template<class DataType>
class OptValue
{
public:
  OptValue() : m_Name(), m_Value(), m_Description(NULL) { }

  OptValue(StringRef pName, DataType pValue, const char* pDescription = NULL)
    : m_Name(pName), m_Value(pValue), m_Description(pDescription) {  }

  const StringRef& getName() const { return m_Name; }

  DataType getValue() const { return (DataType)m_Value; }

  const char* getDescription() const { return m_Description; }

  template<class Opt>
  void apply(Opt& pOption) const { pOption.getParser().addOptValue(*this); }

private:
  StringRef m_Name;
  DataType m_Value;
  const char* m_Description;
};

template<typename DataType>
class OptInit
{
public:
  OptInit(const DataType& pValue) : m_InitValue(pValue) {}

  template<class Opt>
  void apply(Opt& pOption) const { pOption.setValue(m_InitValue); }

private:
  const DataType& m_InitValue;
};

/** \class desc
 *  \brief desc wraps description of an option and pass the description to
 *  OptDefs.
 */
struct desc
{
  desc(const char* pDesc) : mesg(pDesc) { }
  void apply(OptDefs& pOption) const { pOption.setDescription(mesg); }
  const char* mesg;
};

/** \class value_desc
 *  \brief desc wraps description of an option and pass the description to
 *  OptDefs.
 */
struct value_desc
{
  value_desc(const char* pDesc) : mesg(pDesc) { }
  void apply(OptDefs& pOption) const { pOption.setDescription(mesg); }
  const char* mesg;
};

/** \class help
 *  \brief help wraps the help manual of an option. The command line parser
 *  shows the help manual when it fails to parse an option on the command
 *  line.
 */
struct help
{
  help(const char* pHelp) : mesg(pHelp) { }

  void apply(OptDefs& pOption) const { pOption.setHelp(mesg); }

  const char* mesg;
};

struct about
{
  about(AboutData& pAbout) : data(&pAbout) { }

  void apply(OptDefs& pOption) const { data->addBasicOption(pOption); }

  mutable AboutData* data;
};

struct advanced
{
  advanced(AboutData& pAbout) : data(&pAbout) { }

  void apply(OptDefs& pOption) const { data->addAdvOption(pOption); }

  mutable AboutData* data;
};

/// Enum valued command line option
template<typename DataType> OptValue<DataType>
value(StringRef pName, DataType pValue, const char* pDesc = NULL)
{
  return OptValue<DataType>(pName, pValue, pDesc);
}

/// Enum valued command line option
template<typename DataType> OptValue<DataType>
Enum(StringRef pName, DataType pValue, const char* pDesc = NULL)
{
  return OptValue<DataType>(pName, pValue, pDesc);
}

/// Set up initial value
template<typename DataType> OptInit<DataType>
init(const DataType& pValue)
{
  return OptInit<DataType>(pValue);
}

} // namespace of cl
} // namespace of onnc

#endif
