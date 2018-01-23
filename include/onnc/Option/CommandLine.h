//===- CommandLine.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_COMMANDLINE_H
#define ONNC_OPTION_COMMANDLINE_H
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/TypeTraits.h>
#include <onnc/Option/OptDefs.h>
#include <onnc/Option/OptParser.h>
#include <onnc/Option/OptStorage.h>
#include <onnc/Option/Alias.h>
#include <onnc/Option/Bits/Apply.h>
#include <string>

namespace onnc {

/** \namespace onnc::cl
 *  \brief library for defining command line options as LLVM-style.
 *
 * This namespace contains all of the command line option processing machinery.
 * It is intentionally a short name to make qualified usage concise.
 */
namespace cl {

/** \fn ParseCommandLine
 *  \brief Parse arguments on the command line and save the values on the
 *  global variable option. (class opt)
 */
void ParseCommandLine(int pArgc, const char * const *pArgv);

//===----------------------------------------------------------------------===//
// Global Classes
//===----------------------------------------------------------------------===//
/** \class opt
 *  \brief opt class defines an option category on the command line.
 */
template<class DataType,
         class ParserClass = OptParser<DataType> >
class opt : public OptDefs,
            public OptStorage<DataType, is_class<DataType>::value>
{
public:
  template<class M0>
  explicit opt(const M0& pM0) : OptDefs(cl::kOptional) {
    setValueType(type_tag<DataType>());
    apply(pM0, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1>
  opt(const M0& pM0, const M1& pM1)
    : OptDefs(cl::kOptional) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2>
  opt(const M0& pM0, const M1& pM1, const M2& pM2)
    : OptDefs(cl::kOptional) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3>
  opt(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3)
    : OptDefs(cl::kOptional) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3,
           class M4>
  opt(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3,
       const M4& pM4)
    : OptDefs(cl::kOptional) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    apply(pM4, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3,
           class M4, class M5>
  opt(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3,
       const M4& pM4, const M5& pM5)
    : OptDefs(cl::kOptional) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    apply(pM4, this); apply(pM5, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3,
           class M4, class M5, class M6>
  opt(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3,
       const M4& pM4, const M5& pM5, const M6& pM6)
    : OptDefs(cl::kOptional) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    apply(pM4, this); apply(pM5, this); apply(pM6, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3,
           class M4, class M5, class M6, class M7>
  opt(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3,
       const M4& pM4, const M5& pM5, const M6& pM6, const M7& pM7)
    : OptDefs(cl::kOptional) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    apply(pM4, this); apply(pM5, this); apply(pM6, this); apply(pM7, this);
    RegisterOptDefs(*this);
  }

  ParserClass& getParser() { return m_Parser; }

  void print(std::ostream& pOS) const {
    OptStorage<DataType, is_class<DataType>::value>::print(pOS);
  }

private:
  bool handleOccurrence(StringRef pName, StringRef pValue) {
    if (pValue.isValid())
      this->setValueStr(pValue.data());

    typename ParserClass::value_type value =
      typename ParserClass::value_type();

    if (!m_Parser.parse(*this, pName, pValue, value))
      return false;

    this->setValue(value);
    return true;
  }

private:
  ParserClass m_Parser;
};

/** \class list
 *  \brief class list defines an option category which has multiple values
 */
template<class DataType,
         class ParserClass = OptParser<DataType> >
class list : public OptDefs,
             public ListStorage<DataType, is_class<DataType>::value>
{
public:
  // The parameters have arbitrary position. Their order is no matter.
  template<class M0>
  explicit list(const M0& pM0) : OptDefs(cl::kZeroOrMore) {
    setValueType(type_tag<DataType>());
    apply(pM0, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1>
  list(const M0& pM0, const M1& pM1)
    : OptDefs(cl::kZeroOrMore) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2>
  list(const M0& pM0, const M1& pM1, const M2& pM2)
    : OptDefs(cl::kZeroOrMore) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3>
  list(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3)
    : OptDefs(cl::kZeroOrMore) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3,
           class M4>
  list(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3,
       const M4& pM4)
    : OptDefs(cl::kZeroOrMore) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    apply(pM4, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3,
           class M4, class M5>
  list(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3,
       const M4& pM4, const M5& pM5)
    : OptDefs(cl::kZeroOrMore) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    apply(pM4, this); apply(pM5, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3,
           class M4, class M5, class M6>
  list(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3,
       const M4& pM4, const M5& pM5, const M6& pM6)
    : OptDefs(cl::kZeroOrMore) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    apply(pM4, this); apply(pM5, this); apply(pM6, this);
    RegisterOptDefs(*this);
  }

  template<class M0, class M1, class M2, class M3,
           class M4, class M5, class M6, class M7>
  list(const M0& pM0, const M1& pM1, const M2& pM2, const M3& pM3,
       const M4& pM4, const M5& pM5, const M6& pM6, const M7& pM7)
    : OptDefs(cl::kZeroOrMore) {
    setValueType(type_tag<DataType>());
    apply(pM0, this); apply(pM1, this); apply(pM2, this); apply(pM3, this);
    apply(pM4, this); apply(pM5, this); apply(pM6, this); apply(pM7, this);
    RegisterOptDefs(*this);
  }

  ParserClass& getParser() { return m_Parser; }

private:
  typedef typename ListStorage<DataType,
                        is_class<DataType>::value>::option_type OptStorageType;

private:
  bool handleOccurrence(StringRef pName, StringRef pValue) {
    typename ParserClass::value_type value =
      typename ParserClass::value_type();

    if (!m_Parser.parse(*this, pName, pValue, value))
      return false;

    OptStorageType storage;
    storage.setValue(value);
    storage.setValueStr(pValue.data());
    this->addValue(storage);
    return true;
  }

private:
  ParserClass m_Parser;
};

} // namespace of cl
} // namespace of onnc

#endif
