//===- Readline.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_READLINE_H
#define ONNC_SUPPORT_READLINE_H
#include <istream>
#include <string>

namespace onnc {

/** \class Readline
 *  \brief provides a set of functions for use by applications that allow users
 *  to edit command lines as they are typed in.
 */
class Readline
{
public:
  Readline();

  Readline(const std::string& pPrompt);

  ~Readline();

  const std::string& prompt() const { return m_Prompt; }

  /// Add a string in the tab completion
  Readline& complete(const std::string& pValue);

  /// Read a line
  Readline& read(std::istream& pIns, std::string& pLine);

  /// Encounter any fault
  bool isGood() const { return m_bGood; }

  /// read a line
  Readline& operator()(std::istream& pIns, std::string& pLine) {
    return read(pIns, pLine);
  }

  /// Keep reading
  explicit operator bool() const { return this->isGood(); }

private:
  std::string m_Prompt;
  bool m_bGood;
};

} // namespace of onnc

#endif
