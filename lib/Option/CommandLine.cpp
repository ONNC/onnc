//===-- CommandLine.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Option/CommandLine.h>
#include <onnc/Option/OptionPool.h>
#include <onnc/ADT/IList.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;
using namespace onnc::cl;

//===----------------------------------------------------------------------===//
// Registered OptDefs
//===----------------------------------------------------------------------===//
static IList<OptDefs>* GetOptDefsList()
{
  static IList<OptDefs> list;
  return &list;
}

void onnc::cl::RegisterOptDefs(OptDefs& pOption)
{
  GetOptDefsList()->push_back(&pOption);
}

//===----------------------------------------------------------------------===//
// Non Member Functions
//===----------------------------------------------------------------------===//
static inline bool AddOccurrence(OptDefs& pOption,
                                 StringRef pName,
                                 StringRef pValue)
{
  return pOption.addOccurrence(pName, pValue);
}

// Provide the value. The @ref pValue may be embedded in the argument string.
// This function sets up the value of @ref pOption and set @ref pPos to the 
// position of the next option on the command line.
//
// @param[in,out] pOption The option definition which was looked up
// @param[in] pName  The name of the option
// @param[in] pValue The value string of the option
// @param[in] pArgc  The number of arguments on the command line
// @param[in] pArgv  C-style arguments on the command line
// @param[in,out] pPos The position of the option
static inline bool ParseForValues(OptDefs& pOption,
                                  StringRef pName,
                                  StringRef pValue,
                                  int pArgc,
                                  const char* const* pArgv,
                                  unsigned int& pPos)
{
  switch (pOption.getValueOccurrence()) {
    case cl::kValueDisallowed: {
      // pValue has been ready
      if (pValue.isValid()) {
        fatal(pOption, "Argument disallows any value!");
        return false;
      }
      return AddOccurrence(pOption, pName, StringRef("true"));
    }
    case cl::kValueOptional: {
      // get the embedded value.
      if (pValue.isValid())
        return AddOccurrence(pOption, pName, pValue);

      int pos = pPos + 1;
      if (pos >= pArgc || '-' == pArgv[pos][0])
        return AddOccurrence(pOption, pName, StringRef()); //< pValue is invalid.

      ++pPos;
      return AddOccurrence(pOption, pName, pArgv[pPos]);
    }
    case cl::kValueRequired: {
      // get the embedded value.
      if (pValue.isValid())
        return AddOccurrence(pOption, pName, pValue);

      ++pPos;
      if ((int)pPos >= pArgc) {
        fatal(pOption, "argument requires a value!");
        return false;
      }

      if ('-' == pArgv[pPos][0]) {
        fatal(pOption, "argument requires a value!");
        return false;
      }
      return AddOccurrence(pOption, pName, pArgv[pPos]);
    }
    case cl::kValueOneOrMore: {
      // return error if there is no value.
      char delimiter = pOption.getDelimiter();
      if (delimiter == ' ') {
        StringRef arg(pArgv[pPos+1]);
        if (!arg.isValid() || '-' == arg[0]) {
          fatal(pOption, "argument requires at least one value!");
          return false;
        }
      }

      // delimiter is at the last character
      if (pValue.empty()) {
        fatal(pOption, "argument requires at least one value!");
        return false;
      }

      // delimiter doesn't exist
      if (!pValue.isValid()) {
        fatal(pOption, "arguments requires a delimiter");
        return false;
      }
    }
    /** Fall through **/
    case cl::kValueZeroOrMore: {
      // if the delimiter is blank, then go until the next option
      char delimiter = pOption.getDelimiter();
      if (delimiter == ' ') { // blank-separated
        bool result = false;
        while ((int)pPos < pArgc) {
          StringRef arg(pArgv[pPos+1]);
          if (!arg.isValid() || '-' == arg[0])
            return true;
          result |= AddOccurrence(pOption, pName, arg);
          ++pPos;
        }
        return result;
      }

      // if the delimiter is not blank, then separate the string until
      // reach the end.
      StringRef value(pValue);
      StringRef::size_type pos = value.find(delimiter);

      while (pos != StringRef::npos) {
        // Process the portion before the comma.
        if (!AddOccurrence(pOption, pName, value.substr(0, pos)))
          return false;
        // Erase the portion before the comma, AND the comma.
        value = value.substr(pos+1);
        pValue.substr(pos+1); // Increment the original value pointer as well.

        // Check for another comma.
        pos = value.find(delimiter);
      } // end of while
      return AddOccurrence(pOption, pName, value);
    }
    case cl::kGroupValue: {
      // go until the next option
      bool result = false;
      while ((int)pPos < pArgc) {
        StringRef arg(pArgv[pPos]);
        if ('-' == arg[0])
          return true;

        result |= AddOccurrence(pOption, pName, arg);
        ++pPos;
      }
      return result;
    }
    case cl::kSink: {
      // go until the end
      bool result = false;
      while ((int)pPos < pArgc) {
        StringRef arg(pArgv[pPos]);
        result |= AddOccurrence(pOption, pName, arg);
        ++pPos;
      }
      return result;
    }
  } // end of switch
  return true;
}

void onnc::cl::ParseCommandLine(int pArgc, const char* const *pArgv)
{
  // push all registered category in the map
  // collect all kBare options
  // classify via Occurrence for legalization.
  cl::OptionPool pool;
  cl::OptionPool bare_options;
  std::vector<OptDefs*> pos_options;
  std::vector<OptDefs*> existing_options;
  OptDefs* naked_dash = nullptr;
  IList<OptDefs>::iterator cat, cEnd = GetOptDefsList()->end();
  for (cat = GetOptDefsList()->begin(); cat != cEnd; ++cat) {
    if (cl::kPositional == cat->getPrefix())
      pos_options.push_back(cat);
    else if (cl::kBare == cat->getPrefix())
      bare_options.record(*cat);
    else if (cl::kNakedDash == cat->getPrefix())
      naked_dash = cat;
    else
      pool.record(*cat);

    // kRequired and kOneOrMore option must exist
    if (cl::kRequired == cat->getOccurrence() ||
        cl::kOneOrMore == cat->getOccurrence())
      existing_options.push_back(cat);
  }

  // parsing all arguments
  bool eat_all = false;
  std::vector<OptDefs*>::iterator cur_pos = pos_options.begin();
  for (unsigned int pos = 1; (int)pos < pArgc; ++pos) {
    StringRef arg(pArgv[pos]);
    StringRef name;
    StringRef value;

    // get a bare option or a positional option
    if ('-' != arg[0] || (1 < arg.size() && 0 == arg[1]) || eat_all) {
      // if the option can be found in bare_options, it's a bare option
      OptDefs* option = bare_options.lookup(arg, name, value);
      if (NULL != option) {
        ParseForValues(*option, name, value, pArgc, pArgv, pos);
      } //< kBare
      // the option cannot be found in bare_options, it's a positional option
      else {
        if (pos_options.empty()) {
          fatal(opt_no_pos_def) << arg;
          return;
        }
        if (cur_pos == pos_options.end()) {
          fatal(opt_illegal_bare) << arg;
          return;
        }

        AddOccurrence(**cur_pos, name, arg);
        if (cl::kOptional == (*cur_pos)->getOccurrence() ||
            cl::kRequired == (*cur_pos)->getOccurrence())
          ++cur_pos;
      } //< kPositional
    }
    else if (arg.equals("-")) {
      // we get a single '-'
      if (NULL == naked_dash) {
        fatal(opt_undefined_naked_dash);
        return;
      }
      ParseForValues(*naked_dash, name, value, pArgc, pArgv, pos);
    }
    else if (2 < arg.size() && '-' == arg[0] && '-' == arg[1] && 0 == arg[2]) {
      // we get a single '--'. All the following arguments are bare options.
      eat_all = true;
      continue;
    }
    else { // get dash prefix
      // Eat leading dashes.
      StringRef raw_arg(arg);
      while (!arg.empty() && arg[0] == '-')
        arg = arg.substr(1);

      // If a value embeds in the argument string, OptionPool::lookup outputs it.
      OptDefs* option = pool.lookup(arg, name, value);
      if (NULL == option) {
        OptDefs* guess = pool.guess(name, value);
        Rope message;
        if (NULL == guess) {
          fatal(opt_unknown_not_guess) << raw_arg;
          return;
        }
        else { // successfully guess
          std::string prefix;
          switch (guess->getPrefix()) {
            case cl::kParam:
              prefix = "--param ";
              break;
            case cl::kShort:
            case cl::kNakedDash:
            case cl::kGrouped:
              prefix = "-";
              break;
            case cl::kDoubleDash:
            case cl::kLong:
              prefix = "--";
              break;
            case cl::kBare:
            case cl::kPositional:
              break;
          } // end of switch
          fatal(opt_unknown) << raw_arg << prefix << guess->getArgStr();
          return;
        }
        return;
      } // didn't find options

      // provide the value.
      ParseForValues(*option, name, value, pArgc, pArgv, pos);
    } //< get dash prefix
  } // end of trip

  // Legalization
  std::vector<OptDefs*>::iterator existing, eEnd = existing_options.end();
  for (existing = existing_options.begin(); existing != eEnd; ++existing) {
    if (0 == (*existing)->getNumOccurrence()) {
      if (cl::kRequired == (*existing)->getOccurrence())
        fatal(**existing, "option is required");
      if (cl::kOneOrMore == (*existing)->getOccurrence())
        fatal(**existing, "option must be defined more than once");
    }
  }
}
