//===- Option.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPTION_OPTION_H
#define ONNC_OPTION_OPTION_H
#include <onnc/Option/OptDefs.h>
#include <onnc/ADT/IList.h>
#include <onnc/ADT/IListNode.h>

namespace onnc {
namespace cl {

/** \class Option
 *  \brief Option is the occurrence number of an option on the command
 *  line.
 */
class Option : public IListNode<cl::Option>
{
public:
  Option(const OptDefs& pCategory);

  ~Option() { }

  const OptDefs& category() const { return m_Category; }

  unsigned int getNumOccurrence() const { return m_NumOccurrence; }

  bool addOccurrence(StringRef pArgName, StringRef pValue);

  virtual bool handleOccurrence(StringRef pArgName, StringRef pValue) = 0;

private:
  const OptDefs& m_Category;
  unsigned int m_NumOccurrence;
};

} // namespace of cl
} // namespace of onnc

#endif
