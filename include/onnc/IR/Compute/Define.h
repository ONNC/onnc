//===- Define.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_DEFINE_H
#define ONNC_IR_COMPUTE_DEFINE_H
#include <onnc/ADT/StringRef.h>

namespace onnc {

/** \class Define
 */
class Define
{
public:
  Define(StringRef pName)
    : m_Name(pName) {
  }

  virtual ~Define() { }

  StringRef name() const { return m_Name; }

  /// print the operator
  virtual void print(std::ostream& pOS) const = 0;

private:
  StringRef m_Name;
};

} // namespace of onnc

#endif
