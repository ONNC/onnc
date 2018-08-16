//===- OptDefs.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_OPTION_DEFINITIONS_H
#define ONNC_OPTION_OPTION_DEFINITIONS_H
#include <string>
#include <cassert>
#include <onnc/ADT/IListNode.h>
#include <onnc/ADT/TypeTag.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/Rope.h>

namespace onnc {
namespace cl {

/// Flags for the number of occurrences allowed
enum Occurrence
{
  kOptional        = 0x00,  // Zero or One occurrence. (Default)
  kZeroOrMore      = 0x01,  // Zero or more occurrences allowed
  kRequired        = 0x02,  // One occurrence required
  kOneOrMore       = 0x03   // One or more occurrences required
};

/// The format of option
enum Prefix {
  kShort,      ///< Normal option with single dash '-'. (Default)
  kLong,       ///< Normal option with two dashes '--'
  kParam,      ///< Options with argument string.
  kNakedDash,  ///< Single dash option without postfix
  kDoubleDash, ///< Double dash option without postfix
  kBare,       ///< option without dash '-'
  kPositional, ///< Is a positional argument, no '-' required
  kGrouped     ///< Single letter arguments, like `ls -al`.
};

/// The number of value
enum ValueOccurrence {
  kValueDisallowed,  ///< A value can not be specified. (Default)
  kValueOptional,    ///< The value can appear or not. A '=' sign must be provided.
  kValueRequired,    ///< The value is required to appear
  kValueZeroOrMore,  ///< The number of value is zero or more.
  kValueOneOrMore,   ///< The number of value is one or more.
  kGroupValue,       ///< Should this positional cl::list eat -args?
  kSink              ///< Should this positional cl::list eat -args?
};

/// The format of delimiter
enum Delimiter {
  kNoneSeparated  = 0x00, ///< No delimiter. Value and option are attached
                          ///  together. (Default)
  kBlankSeparated = ' ',  ///< Use ' ' as the delimiter
  kEqualSeparated = '=',  ///< Use '=' as the delimiter
  kCommaSeparated = ','   ///< Use ',' as the delimiter
};

/** \class OptDefs
 *  \brief OptDefs stores common information of options in the same group.
 */
class OptDefs : public IListNode<OptDefs>
{
public:
  OptDefs();

  explicit OptDefs(Occurrence pOccurrence);

  OptDefs(const OptDefs& pCopy);

  explicit OptDefs(const std::string& pName);

  OptDefs(const std::string& pName, Occurrence pOccurrence);

  ~OptDefs() { }

  bool hasArgStr() const { return !m_Name.empty(); }

  const char* getArgStr() const { return m_Name.c_str(); }

  void setArgStr(const std::string& pArgStr) { m_Name = pArgStr; }

  const std::string& name() const { return m_Name; }

  /// Occurrence presents the occurrence of an option on the command line.
  Occurrence getOccurrence() const { return m_Occurrence; }

  void setOccurrence(Occurrence pOccur) { m_Occurrence = pOccur; }

  /// The prefix of the option. Default value is kShort
  Prefix getPrefix() const { return m_Prefix; }

  void setPrefix(Prefix pValue) { m_Prefix = pValue; }

  /// The delimiter between option and values.
  Delimiter getDelimiter() const { return m_Delimiter; }

  void setDelimiter(Delimiter pValue) { m_Delimiter = pValue; }

  ValueOccurrence getValueOccurrence() const { return m_ValueOccurrence; }

  void setValueOccurrence(ValueOccurrence pValue) { m_ValueOccurrence = pValue; }

  bool hasDescription() const { return !m_Description.empty(); }

  const std::string& getDescription() const { return m_Description; }

  void setDescription(const std::string& pS) { m_Description = pS; }

  TypeTag getValueType() const { return m_Type; }

  void setValueType(TypeTag pType) { m_Type = pType; }

  /// @retval true If the help message has set.
  bool hasHelp() const { return (NULL != m_Help); }

  const char* getHelp() const { return m_Help; }

  void setHelp(const char* pHelp) { m_Help = pHelp; }

  /// @return The number of occurrence of this option definition
  virtual unsigned int getNumOccurrence() const { return m_NumOccurrence; }

  /// @retval true If this option definition occurs on the command line.
  virtual bool hasOccurrence() const { return (0 != m_NumOccurrence); }

  /// Add occurrence count by one.
  virtual bool addOccurrence(StringRef pArgName, StringRef pValue);

  virtual bool handleOccurrence(StringRef pArgName, StringRef pValue) = 0;

  void print(std::ostream& pOS) const;

private:
  std::string m_Name;
  Occurrence m_Occurrence;
  Prefix m_Prefix;
  Delimiter m_Delimiter;
  ValueOccurrence m_ValueOccurrence;
  std::string m_Description;
  TypeTag m_Type;
  unsigned int m_NumOccurrence;
  const char* m_Help;
};

//===----------------------------------------------------------------------===//
// Helper Functions
//===----------------------------------------------------------------------===//
/// While parsing the command line, message handler isn't initialized. Therefore,
/// we need a special fatal function to print error message
void fatal(const OptDefs& pOption, Rope pMessage);

/// While parsing the command line, message handler isn't initialized. Therefore,
/// we need a special error function to print error message
void error(const OptDefs& pOption, Rope pMessage);

/// Register OptDefs to the global option pool.
void RegisterOptDefs(OptDefs& pOption);

} // namespace of cl
} // namespace of onnc

#endif
