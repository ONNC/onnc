//===-- OptDefs.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Option/OptDefs.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <iomanip>

using namespace onnc;
using namespace onnc::cl;

//===----------------------------------------------------------------------===//
// Message Handling
//===----------------------------------------------------------------------===//
static inline std::string getArguName(const OptDefs& pOption)
{
  std::string argu;
  switch (pOption.getPrefix()) {
    case cl::kNakedDash:
    case cl::kShort: {
      argu += "-";
      break;
    }
    case cl::kLong:
    case cl::kDoubleDash: {
      argu += "--";
      break;
    }
    case cl::kParam: {
      argu += "--param ";
      break;
    }
    default:
      break;
  }

  if (pOption.hasArgStr())
    argu += pOption.getArgStr();

  return argu;
}

void cl::fatal(const OptDefs& pOption, Rope pMessage)
{
  std::string argu = getArguName(pOption);
  if (pOption.hasHelp())
    fatal(illegal_option_help) << argu << pMessage.str() << pOption.getHelp();
  else
    fatal(illegal_option) << argu << pMessage.str();
}

void cl::error(const OptDefs& pOption, Rope pMessage)
{
  std::string argu = getArguName(pOption);
  error(illegal_option) << argu << pMessage.str();
}

//===----------------------------------------------------------------------===//
// OptDefs
//===----------------------------------------------------------------------===//
OptDefs::OptDefs()
  : m_Name(),
    m_Occurrence(kOptional),
    m_Prefix(kShort),
    m_Delimiter(kBlankSeparated),
    m_ValueOccurrence(kValueDisallowed),
    m_Description(),
    m_Type(type_tag<bool>()),
    m_NumOccurrence(0),
    m_Help(nullptr) {
}

OptDefs::OptDefs(Occurrence pOccur)
  : m_Name(),
    m_Occurrence(pOccur),
    m_Prefix(kShort),
    m_Delimiter(kBlankSeparated),
    m_ValueOccurrence(kValueDisallowed),
    m_Description(),
    m_Type(type_tag<bool>()),
    m_NumOccurrence(0),
    m_Help(nullptr) {
}

OptDefs::OptDefs(const std::string& pName)
  : m_Name(pName),
    m_Occurrence(kOptional),
    m_Prefix(kShort),
    m_Delimiter(kBlankSeparated),
    m_ValueOccurrence(kValueDisallowed),
    m_Description(),
    m_Type(type_tag<bool>()),
    m_NumOccurrence(0),
    m_Help(nullptr) {
}

OptDefs::OptDefs(const std::string& pName, Occurrence pOccur)
  : m_Name(pName),
    m_Occurrence(pOccur),
    m_Prefix(kShort),
    m_Delimiter(kBlankSeparated),
    m_ValueOccurrence(kValueDisallowed),
    m_Description(),
    m_Type(type_tag<bool>()),
    m_NumOccurrence(0),
    m_Help(nullptr) {
}

OptDefs::OptDefs(const OptDefs& pCopy)
  : IListNode<OptDefs>(),
    m_Name(pCopy.m_Name),
    m_Occurrence(pCopy.m_Occurrence),
    m_Prefix(pCopy.m_Prefix),
    m_Delimiter(pCopy.m_Delimiter),
    m_ValueOccurrence(pCopy.m_ValueOccurrence),
    m_Description(pCopy.m_Description),
    m_Type(pCopy.m_Type),
    m_NumOccurrence(pCopy.m_NumOccurrence),
    m_Help(pCopy.m_Help) {
  // Do not copy IListNode linkage.
}

//===----------------------------------------------------------------------===//
// Print Out functions
//===----------------------------------------------------------------------===//
inline static void PrintValueType(std::string& pArgu, TypeTag pValueType)
{
  if (pValueType == type_tag<int>() ||
      pValueType == type_tag<unsigned int>()) {
    pArgu += "number";
    return;
  }
  if (pValueType == type_tag<float>() ||
      pValueType == type_tag<double>()) {
    pArgu += "floating";
    return;
  }
  if (pValueType == type_tag<StringRef>() ||
      pValueType == type_tag<std::string>() ||
      pValueType == type_tag<const char*>()) {
    pArgu += "string";
    return;
  }
  if (pValueType == type_tag<bool>()) {
    pArgu += "boolean";
    return;
  }
  if (pValueType == type_tag<Path>()) {
    pArgu += "path";
    return;
  }
}

void OptDefs::print(std::ostream& pOS) const
{
  pOS << std::setw(13) << std::left;
  std::string argu;
  switch (m_Prefix) {
    case cl::kShort:
      argu += "-";
      argu += m_Name;
      break;
    case cl::kLong:
      argu += "--";
      argu += m_Name;
      break;
    case cl::kNakedDash:
      argu += "-";
      break;
    case cl::kDoubleDash:
      argu += "--";
      break;
    case cl::kParam:
      argu += "--param ";
      argu += m_Name;
      break;
    default:
      argu += m_Name;
      break;
  }
  pOS << argu;

  pOS << std::setw(13) << std::left;
  std::string value;
  switch (m_Delimiter) {
    case cl::kNoneSeparated:
    case cl::kBlankSeparated:
      break;
    case cl::kEqualSeparated:
      value += "=";
      break;
    case cl::kCommaSeparated:
      value += ",";
      break;
  }

  switch (m_ValueOccurrence) {
    case cl::kValueDisallowed:
      break;
    case cl::kValueOptional:
    case cl::kValueRequired:
      value += "<";
      PrintValueType(value, m_Type);
      value += ">";
      break;
    case cl::kValueZeroOrMore:
    case cl::kValueOneOrMore:
    case cl::kGroupValue:
    case cl::kSink:
      value += "<";
      PrintValueType(value, m_Type);
      value += "...>";
      break;
  }
  pOS << value;
  pOS << std::setw(50) << std::left << m_Description << std::endl;
}

bool OptDefs::addOccurrence(StringRef pName, StringRef pValue)
{
  ++m_NumOccurrence;

  // check number of occurrences.
  // We keep the checking here rather than legalization phase because the
  // errors of occurrence it better to be recognized as early as possible.
  switch (getOccurrence()) {
    case cl::kOptional:
      if (m_NumOccurrence > 1) {
        error(*this, "may only occur zero or one times!");
        return false;
      }
      break;
    case cl::kRequired:
      if (m_NumOccurrence > 1) {
        error(*this, "may occur exactly once!");
        return false;
      }
      break;
    case cl::kZeroOrMore:
    case cl::kOneOrMore:
      break;
  }

  return this->handleOccurrence(pName, pValue);
}
