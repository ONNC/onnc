//===- Expansion.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_EXPANSION_H
#define ONNC_SUPPORT_EXPANSION_H
#include <onnc/ADT/StringList.h>

namespace onnc {

class ExpansionImpl;

/** \class Expansion
 *  \brief Expansion performs shell-style word expansion.
 *
 *  Expansion allocates memory for all expanded strings. It must be alive
 *  till expanded word expired.
 *  The expansion done consists of the following stages:
 *   - tilde expansion (replacing ~user by user's home directory),
 *   - variable substitution (replacing $FOO by the value of the environment
 *     variable FOO),
 *   - command substitution (replacing $(command) or `command` by the output
 *     of command),
 *   - arithmetic expansion $((expression)),
 *   - field splitting,
 *   - wildcard expansion,
 *   - quote removal. 
 */
class Expansion
{
public:
  enum ErrorType {
    kSuccess,   ///< No error
    kBadChar,   ///< The words argument contains one of the following unquoted
                ///  characters: <newline>, `|', `&', `;', `<', `>', `(', `)', `{', `}'
    kNoSpace,   ///< Not enough memory to store the result
    kSyntaxErr, ///< Shell syntax error in @ref pWord
    kUnknown    ///< Unknown error
  };

public:
  Expansion(StringRef pWord);

  ~Expansion();

  /// The count of matched words
  unsigned int size() const;

  /// The list of expanded words
  StringList words() const;

  /// return true only if error() returns kSuccess
  bool isGood() const { return (kSuccess == this->error()); }

  /// return error if any.
  ErrorType error() const;

private:
  friend class ExpansionImpl;

  ExpansionImpl* m_pImpl;
};

} // namespace of onnc

#endif
