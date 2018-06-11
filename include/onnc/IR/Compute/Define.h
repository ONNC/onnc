//===- Define.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_DEFINE_H
#define ONNC_IR_COMPUTE_DEFINE_H
#include <string>
#include <onnc/ADT/StringRef.h>

namespace onnc {

class Define
{
public:
  Define(StringRef p_name): m_Name(p_name) {}

  StringRef getName();

private:
  std::string m_Name;
};

} // namespace of onnc

#endif
